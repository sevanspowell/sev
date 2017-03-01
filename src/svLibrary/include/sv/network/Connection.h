//===-- sv/network/Connection.h - Virtual connection ------------*- C++ -*-===//
//
//                 The Special Engine Variant Game Engine
//
// This file is distributed under the MIT License. See LICENSE.txt for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Fast TCP-like connection implemented using UDP.
///
/// Because UDP sockets are connectionless and can receive packets sent from any
/// computer, a protocol id is used to identify packets that the connection is
/// interested in. All other packets received are ignored.
///
/// A connection is considered to be establisehd as soon as we receive a packet
/// with the correct protocol id. When the server receives the first packet from
/// the client, it takes note of the IP address and port of the client and sends
/// packets back to that IP address and port. The server will then begin
/// filtering out packets that don't match the client's IP address/port
/// combination, simulating a direct connection between the two.
///
/// The client acts similiarly, it already knows the server's address (specified
/// on connect) so it merely filters out packets that don't come from the server
/// address.
///
/// We define disconnection as not receiving packets from the other end of the
/// connection for 'timeout' seconds.
///
/// Typical usage:
///     int16_t clientPort = 30001;
///     int16_t serverPort = 30000;
///
///     Connection client(id, timeout);
///     Connection server(id, timeout);
///
///     client.start(clientPort);
///     server.start(serverPort);
///
///     client.connect(Address(127, 0, 0, 1, serverPort));
///     server.listen();
///
///     ...
///     client.sendPacket(...);
///     server.receivePacket(...);
///
///     client.update(deltaTime);
///     server.update(deltaTime);
///
/// Based off the code provided by Glenn Fiedler
/// http://gafferongames.com/networking-for-game-programmers/virtual-connection-over-udp/
///
//===----------------------------------------------------------------------===//
#pragma once

#include <sv/network/Sockets.h>

namespace sv {
namespace net {
namespace ConnectionMode {
enum Enum { None, Client, Server };
}

namespace ConnectionState {
enum Enum { Disconnected, Listening, Connecting, ConnectFail, Connected };
}

/// Virtual network connection
class Connection {
  public:
    ///-------------------------------------------------------------------------
    /// Construct a connection.
    ///
    /// \param   protocolId   32-bit id used to identify packets this connection
    /// is interested in. Packets received that don't have this id in their
    /// first four bytes will be discarded
    /// \param   timeout   How long to wait between receiving packets before
    /// considering the connection terminated (in seconds).
    ///-------------------------------------------------------------------------
    Connection(uint32_t protocolId_, float timeout_)
        : protocolId(protocolId_), timeout(timeout_),
          mode(ConnectionMode::Enum::None), isRunning(false) {
        clearData();
    }

    ///-------------------------------------------------------------------------
    /// Destroy a connection.
    ///-------------------------------------------------------------------------
    ~Connection();

    ///-------------------------------------------------------------------------
    /// Open the connection on the given \p port.
    ///-------------------------------------------------------------------------
    bool start(int16_t port);

    ///-------------------------------------------------------------------------
    /// Close the connection.
    ///-------------------------------------------------------------------------
    void stop();

    ///-------------------------------------------------------------------------
    /// Begin listening for a client connection.
    ///-------------------------------------------------------------------------
    void listen();

    ///-------------------------------------------------------------------------
    /// Connect to a server.
    ///-------------------------------------------------------------------------
    void connect(const Address &serverAddress);

    ///-------------------------------------------------------------------------
    /// \returns If this connection is currently acting as a client, a server or
    /// neither.
    ///-------------------------------------------------------------------------
    ConnectionMode::Enum getMode() const;

    ///-------------------------------------------------------------------------
    /// \returns The current state of the connection.
    ///-------------------------------------------------------------------------
    ConnectionState::Enum getState() const;

    ///-------------------------------------------------------------------------
    /// Call this regularly to keep the state of the connection up-to-date.
    ///
    /// \param   deltaTime   Time since last update in seconds.
    ///-------------------------------------------------------------------------
    void update(float deltaTime);

    ///-------------------------------------------------------------------------
    /// Send a packet over the connnection.
    ///
    /// NOTE: Protocol id is prepended to the packet automatically.
    ///
    /// \returns True if packet sent successfully, false otherwise.
    ///-------------------------------------------------------------------------
    bool sendPacket(const void *data, size_t dataSize);

    ///-------------------------------------------------------------------------
    /// Receive a packet over the connection.
    ///
    /// \returns Number of bytes read (excludes protocol id).
    ///-------------------------------------------------------------------------
    size_t receivePacket(void *buffer, size_t bufferSize);

  private:
    // Convenience method to clear some internal state
    void clearData();

    uint32_t protocolId;
    float timeout;

    bool isRunning;
    ConnectionMode::Enum mode;
    ConnectionState::Enum state;
    Socket socket;
    float timeoutAccumulator;
    Address address;
};
}
}
