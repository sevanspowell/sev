//===-- sv/network/Sockets.h - Sockets layer --------------------*- C++ -*-===//
//
//                 The Special Engine Variant Game Engine
//
// This file is distributed under the MIT License. See LICENSE.txt for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Abstract sockets on various platforms.
///
/// Based off the code provided by Glenn Fiedler
/// http://gafferongames.com/networking-for-game-programmers/sending-and-receiving-packets/
///
//===----------------------------------------------------------------------===//
#pragma once

#include <cstdint>

namespace sv {
namespace net {
///-----------------------------------------------------------------------------
/// Initialize the sockets layer.
///
/// NOTE: Must be called before anything in the sockets layer is used.
///
/// \returns True if initialized successfully, false otherwise.
///-----------------------------------------------------------------------------
bool initializeSockets();

///-----------------------------------------------------------------------------
/// Shutdown the sockets layer.
///
/// NOTE: Must be called when done with the sockets layer.
///-----------------------------------------------------------------------------
void shutdownSockets();

/// Represents a network address
class Address {
  public:
    Address() : address(0), port(0) {}
    Address(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint16_t port_)
        : address((a << 24) | (b << 16) | (c << 8) | d), port(port_) {}
    Address(uint32_t address_, uint16_t port_)
        : address(address_), port(port_) {}

    ///-------------------------------------------------------------------------
    /// Get the IP address
    ///-------------------------------------------------------------------------
    uint32_t getAddress() const;

    /// Get the first component of the IP address
    uint8_t getA() const;
    /// Get the second component of the IP address
    uint8_t getB() const;
    /// Get the third component of the IP address
    uint8_t getC() const;
    /// Get the fourth component of the IP address
    uint8_t getD() const;

    /// Get the address port
    uint16_t getPort() const;

    bool operator==(const Address &other) const;
    bool operator!=(const Address &other) const;

  private:
    uint32_t address;
    uint16_t port;
};

/// A UDP (connectionless, unreliable) socket
class Socket {
  public:
    Socket() : socket(0) {}
    ~Socket();

    ///-------------------------------------------------------------------------
    /// Sets the socket up to listen on the given port.
    ///
    /// NOTE: Passing 0 for port number will result in system selecting a free
    /// port.
    ///
    /// \returns True if socket was opened successfully, false otherwise.
    ///-------------------------------------------------------------------------
    bool open(uint16_t port);

    ///-------------------------------------------------------------------------
    /// Stop the socket listening on the currently open port.
    ///-------------------------------------------------------------------------
    void close();

    ///-------------------------------------------------------------------------
    /// \returns True if this socket is listening on a port, false otherwise.
    ///-------------------------------------------------------------------------
    bool isOpen() const;

    ///-------------------------------------------------------------------------
    /// Send some data over this socket to the given address.
    ///
    /// \pre Socket must be opened, use 'isOpen' to check.
    ///
    /// \returns True if successfully sent, false otherwise. DOES NOT indicate
    /// if data was received successfully or not.
    ///-------------------------------------------------------------------------
    bool send(const Address &destination, const void *data, size_t size);

    ///-------------------------------------------------------------------------
    /// Try to receive some data from the given sender.
    ///
    /// \pre Socket must be opened, use 'isOpen' to check.
    ///
    /// \param   sender       IP address of sender to receive data from.
    /// \param   buffer       Buffer to receive data into.
    /// \param   bufferSize   Maximum amount of data the buffer can receive.
    /// Packets with more data than this limit are silently discarded.
    ///
    /// \returns Number of bytes read.
    ///-------------------------------------------------------------------------
    size_t receive(Address &sender, void *buffer, size_t bufferSize);

  private:
    int32_t socket;
};
}
}
