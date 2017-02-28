#include <iostream>

#include <sv/System.h>
#include <sv/network/Sockets.h>

TEST(Address, Defaults) {
    sv::net::Address address;

    EXPECT_EQ(address.getA(), 0);
    EXPECT_EQ(address.getB(), 0);
    EXPECT_EQ(address.getC(), 0);
    EXPECT_EQ(address.getD(), 0);
    EXPECT_EQ(address.getPort(), 0);
    EXPECT_EQ(address.getAddress(), 0);
}

TEST(Address, Constructors) {
    const uint8_t a     = 100;
    const uint8_t b     = 110;
    const uint8_t c     = 50;
    const uint8_t d     = 12;
    const uint16_t port = 10000;

    sv::net::Address address(a, b, c, d, port);

    EXPECT_EQ(address.getA(), a);
    EXPECT_EQ(address.getB(), b);
    EXPECT_EQ(address.getC(), c);
    EXPECT_EQ(address.getD(), d);
    EXPECT_EQ(address.getPort(), port);

    address = sv::net::Address((a << 24) | (b << 16) | (c << 8) | d, port);

    EXPECT_EQ(address.getA(), a);
    EXPECT_EQ(address.getB(), b);
    EXPECT_EQ(address.getC(), c);
    EXPECT_EQ(address.getD(), d);
    EXPECT_EQ(address.getPort(), port);
}

TEST(Address, EqualityInequality) {
    sv::net::Address x(100, 110, 0, 1, 50000);
    sv::net::Address y(101, 210, 6, 5, 50002);

    EXPECT_TRUE(x != y);
    EXPECT_TRUE(x == x);
    EXPECT_TRUE(y == y);
}

TEST(Socket, OpenClose) {
    sv::net::initializeSockets();

    sv::net::Socket socket;
    EXPECT_FALSE(socket.isOpen());
    EXPECT_TRUE(socket.open(30000));
    EXPECT_TRUE(socket.isOpen());
    socket.close();
    EXPECT_FALSE(socket.isOpen());
    EXPECT_TRUE(socket.open(30000));
    EXPECT_TRUE(socket.isOpen());
    socket.close();

    sv::net::shutdownSockets();
}

TEST(Socket, FailsOnSamePort) {
    sv::net::initializeSockets();

    sv::net::Socket a, b;
    EXPECT_TRUE(a.open(30000));
    EXPECT_FALSE(b.open(30000));
    EXPECT_TRUE(a.isOpen());
    EXPECT_FALSE(b.isOpen());
    a.close();

    sv::net::shutdownSockets();
}

TEST(Socket, SendAndRecvPackets) {
    sv::net::initializeSockets();

    sv::net::Socket a, b;
    int16_t port1 = 30000;
    int16_t port2 = 30001;
    EXPECT_TRUE(a.open(port1));
    EXPECT_TRUE(b.open(port2));

    const char packet[] = "packet data";
    bool aRecvdPacket   = false;
    bool bRecvdPacket   = false;

    while (!aRecvdPacket && !bRecvdPacket) {
        EXPECT_TRUE(a.send(sv::net::Address(127, 0, 0, 1, port2), packet,
                           sizeof(packet)));
        EXPECT_TRUE(b.send(sv::net::Address(127, 0, 0, 1, port1), packet,
                           sizeof(packet)));

        while (true) {
            sv::net::Address sender;
            char buffer[256];
            int bytesRead = a.receive(sender, buffer, sizeof(buffer));
            if (bytesRead == 0) {
                break;
            }
            if (bytesRead == sizeof(packet) && strcmp(buffer, packet) == 0) {
                aRecvdPacket = true;
            }

            // A receives from B (open on port 2)
            EXPECT_EQ(sender, sv::net::Address(127, 0, 0, 1, port2));
        }

        while (true) {
            sv::net::Address sender;
            char buffer[256];
            int bytesRead = b.receive(sender, buffer, sizeof(buffer));
            if (bytesRead == 0) {
                break;
            }
            if (bytesRead == sizeof(packet) && strcmp(buffer, packet) == 0) {
                bRecvdPacket = true;
            }

            // B receives from A (open on port 1)
            EXPECT_EQ(sender, sv::net::Address(127, 0, 0, 1, port1));
        }
    }

    EXPECT_TRUE(aRecvdPacket);
    EXPECT_TRUE(bRecvdPacket);

    sv::net::shutdownSockets();
}
