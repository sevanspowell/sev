#include <sv/System.h>

#if SV_PLATFORM_WINDOWS
#include <winsock2.h>
#elif SV_PLATFORM_POSIX
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

#include <sv/Globals.h>
#include <sv/network/Sockets.h>

namespace sv {
namespace net {
bool initializeSockets() {
#if SV_PLATFORM_WINDOWS
    WSADATA wsaData;
    return WSAStartup(MAKEWORD(2, 2), &wsaData) == NO_ERROR;
#elif SV_PLATFORM_POSIX
    return true; // POSIX sockets don't need to be initialized
#else
    return false;
#endif
}

void shutdownSockets() {
#if SV_PLATFORM_WINDOWS
    WSACleanup();
#endif
}

uint32_t Address::getAddress() const { return address; }

uint8_t Address::getA() const { return (uint8_t)(address >> 24); }

uint8_t Address::getB() const { return (uint8_t)(address >> 16); }

uint8_t Address::getC() const { return (uint8_t)(address >> 8); }

uint8_t Address::getD() const { return (uint8_t)(address); }

uint16_t Address::getPort() const { return port; }

bool Address::operator==(const Address &other) const {
    return (address == other.address) && (port == other.port);
}

bool Address::operator!=(const Address &other) const {
    return !(*this == other);
}

Socket::~Socket() { close(); }

bool Socket::open(uint16_t port) {
    // Is open already?
    if (isOpen()) {
        close();
    }

    // Open socket
    socket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (socket <= 0) {
        globals::log(LogArea::Enum::Network, LogLevel::Enum::Error,
                     "Failed to create socket.");
        return false;
    }

    // Bind socket to port
    sockaddr_in address;
    address.sin_family      = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port =
        htons(port); // htons - host byte order to network byte order

    if (bind(socket, (const sockaddr *)&address, sizeof(sockaddr_in)) < 0) {
        close();
        globals::log(LogArea::Enum::Network, LogLevel::Enum::Error,
                     "Failed to bind socket.");
        return false;
    }

// Set socket to non-blocking
#if SV_PLATFORM_POSIX
    int nonBlocking = 1;
    if (fcntl(socket, F_SETFL, O_NONBLOCK, nonBlocking) == -1) {
        close();
        globals::log(LogArea::Enum::Network, LogLevel::Enum::Error,
                     "Failed to set non-blocking.");
        return false;
    }
#elif
    DWORD nonBlocking = 1;
    if (ioctlsocket(socket, FIONBIO, &nonBlocking) != 0) {
        close();
        globals::log(LogArea::Enum::Network, LogLevel::Enum::Error,
                     "Failed to set non-blocking.");
        return false;
    }
#else
    close();
    return false;
#endif

    return true;
}

void Socket::close() {
    // Only close an open socket
    if (isOpen()) {
#if SV_PLATFORM_POSIX
        ::close(socket);
#elif SV_PLATFORM_WINDOWS
        closesocket(socket);
#endif
        socket = 0;
    }
}

bool Socket::isOpen() const { return socket > 0; }

bool Socket::send(const Address &destination, const void *data, size_t size) {
    if (data != nullptr && size > 0) {
        if (!isOpen()) {
            return false;
        }

        // Setup destination address structure
        sockaddr_in destAddr;
        destAddr.sin_family      = AF_INET;
        destAddr.sin_addr.s_addr = htonl(destination.getAddress());
        destAddr.sin_port        = htons(destination.getPort());

        // Send data
        int sentBytes = sendto(socket, (const char *)data, size, 0,
                               (sockaddr *)&destAddr, sizeof(sockaddr_in));

        // Check for errors
        if (sentBytes != size) {
            globals::log(LogArea::Enum::Network, LogLevel::Enum::Error,
                         "Failed to send packet.");
            return false;
        }
    }

    // No errors
    return true;
}

size_t Socket::receive(Address &sender, void *buffer, size_t bufferSize) {
    size_t receivedBytes = 0;

#if SV_PLATFORM_WINDOWS
    typedef int socklen_t;
#endif

    if (buffer != nullptr && bufferSize > 0) {
        // To retrieve where the packet came from
        sockaddr_in from;
        socklen_t fromLength = sizeof(from);

        // Retrieve packet
        int bytesRead = recvfrom(socket, (char *)buffer, bufferSize, 0,
                                 (sockaddr *)&from, &fromLength);

        // Return address to caller
        uint32_t fromAddress = ntohl(from.sin_addr.s_addr);
        uint16_t fromPort    = ntohs(from.sin_port);
        sender               = Address(fromAddress, fromPort);

        if (bytesRead <= 0) {
            receivedBytes = 0;
        } else {
            receivedBytes = bytesRead;
        }
    }

    return receivedBytes;
}
}
}
