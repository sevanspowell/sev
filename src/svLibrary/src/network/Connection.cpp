#include <cstring>

#include <sv/Globals.h>
#include <sv/network/Connection.h>

namespace sv {
namespace net {
Connection::~Connection() {
    if (isRunning == true) {
        stop();
    }
}

bool Connection::start(int16_t port) {
    bool result = false;

    if (isRunning == false) {
        if (socket.open(port) != false) {
            isRunning = true;
            result    = true;
        }
    } else {
        sv::globals::log(
            LogArea::Enum::Network, LogLevel::Enum::Warning,
            "Tried to start connection already running on a port.");
    }

    return result;
}

void Connection::stop() {
    if (isRunning) {
        clearData();
        socket.close();
        isRunning = false;
    }
}

void Connection::listen() {
    clearData();
    mode  = ConnectionMode::Enum::Server;
    state = ConnectionState::Enum::Listening;
}

void Connection::connect(const Address &serverAddress) {
    clearData();
    mode    = ConnectionMode::Enum::Client;
    state   = ConnectionState::Enum::Connecting;
    address = serverAddress;
}

ConnectionMode::Enum Connection::getMode() const { return mode; }

ConnectionState::Enum Connection::getState() const { return state; }

void Connection::update(float deltaTime) {
    if (isRunning) {
        timeoutAccumulator += deltaTime;
        // If we reached timeout
        if (timeoutAccumulator > timeout) {
            if (state == ConnectionState::Enum::Connecting) {
                clearData();
                state = ConnectionState::Enum::ConnectFail;
                sv::globals::log(LogArea::Enum::Network,
                                 LogLevel::Enum::Warning,
                                 "Connection timed out.");
            } else if (state == ConnectionState::Enum::Connected) {
                clearData();
                state = ConnectionState::Enum::ConnectFail;
                sv::globals::log(LogArea::Enum::Network,
                                 LogLevel::Enum::Warning,
                                 "Connection timed out.");
            }
        }
    }
}

bool Connection::sendPacket(const void *data, size_t dataSize) {
    bool result = false;

    if (isRunning) {
        if (address.getAddress() != 0) {
            uint8_t packet[dataSize + 4];
            // Copy protocol id to packet
            packet[0] = (uint8_t)(protocolId >> 24);
            packet[1] = (uint8_t)((protocolId >> 16) & 0xFF);
            packet[2] = (uint8_t)((protocolId >> 8) & 0xFF);
            packet[3] = (uint8_t)(protocolId & 0xFF);
            memcpy(&packet[4], data, dataSize);

            // Send packet
            result = socket.send(address, packet, dataSize + 4);
        }
    }

    return result;
}

size_t Connection::receivePacket(void *buffer, size_t bufferSize) {
    size_t bytesReceived = 0;

    if (isRunning) {
        uint8_t packet[bufferSize + 4];

        Address sender;
        size_t bytesRead = socket.receive(sender, packet, bufferSize + 4);

        // If we recognize the first four bytes as the protocolId
        if (bytesRead > 4 &&
            (packet[0] == (uint8_t)(protocolId >> 24) &&
             (packet[1] == (uint8_t)((protocolId >> 16) & 0xFF)) &&
             (packet[2] == (uint8_t)((protocolId >> 8) & 0xFF)) &&
             (packet[3] == (uint8_t)(protocolId & 0xFF)))) {

            if (mode == ConnectionMode::Enum::Server &&
                state != ConnectionState::Enum::Connected) {
                // Server accepts connection from client
                state = ConnectionState::Enum::Connected;
                // Keep track of who sent the packet
                address = sender;
            }

            // If sender matches the server/client we have/want a connection
            // with
            if (sender == address) {
                if (mode == ConnectionMode::Enum::Client &&
                    state == ConnectionState::Enum::Connecting) {
                    // Client completes connection with server
                    state = ConnectionState::Enum::Connected;
                }

                // We successfully received a packet, reset timeout accumulator
                timeoutAccumulator = 0.0f;
                memcpy(buffer, &packet[4], bufferSize - 4);

                bytesReceived = bufferSize - 4;
            }
        }
    }

    return bytesReceived;
}

void Connection::clearData() {
    state              = ConnectionState::Enum::Disconnected;
    timeoutAccumulator = 0.0f;
    address            = Address();
}
}
}
