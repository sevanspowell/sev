// Modified from code provided by Glenn Fiedler (gafferongames.com)
#include <cstring>

#include <sv/Common.h>
#include <sv/network/Connection.h>

TEST(Connection, TestJoin) {
    EXPECT_TRUE(sv::net::initializeSockets());

    const int16_t serverPort = 30000;
    const int16_t clientPort = 30001;
    const int32_t protocolId = 0x11112222;
    const float deltaTime    = 0.001f;
    const float timeout      = 0.1f;

    sv::net::Connection client(protocolId, timeout);
    sv::net::Connection server(protocolId, timeout);

    EXPECT_TRUE(client.start(clientPort));
    EXPECT_TRUE(server.start(serverPort));

    client.connect(sv::net::Address(127, 0, 0, 1, serverPort));
    server.listen();

    while (true) {
        sv::net::ConnectionState::Enum clientState = client.getState();
        sv::net::ConnectionState::Enum serverState = server.getState();

        // A connection is only established when a packet is successfully
        // received, so we are done whenever the connection is considered
        // established on both ends.
        if (clientState == sv::net::ConnectionState::Enum::Connected &&
            serverState == sv::net::ConnectionState::Enum::Connected) {
            break;
        }

        if (clientState != sv::net::ConnectionState::Enum::Connecting &&
            clientState == sv::net::ConnectionState::Enum::ConnectFail) {
            break;
        }

        uint8_t clientPacket[] = "client to server";
        client.sendPacket(clientPacket, sizeof(clientPacket));
        uint8_t serverPacket[] = "server to client";
        server.sendPacket(serverPacket, sizeof(serverPacket));

        while (true) {
            uint8_t packet[256];
            size_t bytesRead = client.receivePacket(packet, sizeof(packet));
            if (bytesRead == 0) {
                break;
            } else {
                EXPECT_TRUE(strcmp("server to client", (const char *)packet) ==
                            0);
            }
        }

        while (true) {
            uint8_t packet[256];
            size_t bytesRead = server.receivePacket(packet, sizeof(packet));
            if (bytesRead == 0) {
                break;
            } else {
                EXPECT_TRUE(strcmp("client to server", (const char *)packet) ==
                            0);
            }
        }

        client.update(deltaTime);
        server.update(deltaTime);

        sv::sleep(deltaTime);
    }

    EXPECT_TRUE(client.getState() == sv::net::ConnectionState::Enum::Connected);
    EXPECT_TRUE(server.getState() == sv::net::ConnectionState::Enum::Connected);

    sv::net::shutdownSockets();
}

TEST(Connection, TestJoinTimeout) {
    EXPECT_TRUE(sv::net::initializeSockets());

    const int16_t serverPort = 30000;
    const int16_t clientPort = 30001;
    const int32_t protocolId = 0x11112222;
    const float deltaTime    = 0.001f;
    const float timeout      = 0.1f;

    sv::net::Connection client(protocolId, timeout);

    EXPECT_TRUE(client.start(clientPort));

    client.connect(sv::net::Address(127, 0, 0, 1, serverPort));

    while (true) {
        if (client.getState() != sv::net::ConnectionState::Enum::Connecting) {
            break;
        }

        uint8_t clientPacket[] = "client to server";
        client.sendPacket(clientPacket, sizeof(clientPacket));

        while (true) {
            uint8_t packet[256];
            size_t bytesRead = client.receivePacket(packet, sizeof(packet));
            if (bytesRead == 0) {
                break;
            }
        }

        client.update(deltaTime);

        sv::sleep(deltaTime);
    }

    EXPECT_TRUE(client.getState() != sv::net::ConnectionState::Enum::Connected);
    EXPECT_TRUE(client.getState() ==
                sv::net::ConnectionState::Enum::ConnectFail);

    sv::net::shutdownSockets();
}

TEST(Connection, TestJoinBusy) {
    EXPECT_TRUE(sv::net::initializeSockets());

    const int16_t serverPort = 30000;
    const int16_t clientPort = 30001;
    const int32_t protocolId = 0x11112222;
    const float deltaTime    = 0.001f;
    const float timeout      = 0.1f;

    sv::net::Connection client(protocolId, timeout);
    sv::net::Connection server(protocolId, timeout);

    EXPECT_TRUE(client.start(clientPort));
    EXPECT_TRUE(server.start(serverPort));

    client.connect(sv::net::Address(127, 0, 0, 1, serverPort));
    server.listen();

    // Establish connection between server and client
    while (true) {
        if (client.getState() == sv::net::ConnectionState::Enum::Connected &&
            server.getState() == sv::net::ConnectionState::Enum::Connected) {
            break;
        }

        if (client.getState() != sv::net::ConnectionState::Enum::Connecting &&
            client.getState() == sv::net::ConnectionState::Enum::ConnectFail) {
            break;
        }

        uint8_t clientPacket[] = "client to server";
        client.sendPacket(clientPacket, sizeof(clientPacket));
        uint8_t serverPacket[] = "server to client";
        server.sendPacket(serverPacket, sizeof(serverPacket));

        while (true) {
            uint8_t packet[256];
            size_t bytesRead = client.receivePacket(packet, sizeof(packet));
            if (bytesRead == 0) {
                break;
            }
        }

        while (true) {
            uint8_t packet[256];
            size_t bytesRead = server.receivePacket(packet, sizeof(packet));
            if (bytesRead == 0) {
                break;
            }
        }

        client.update(deltaTime);
        server.update(deltaTime);

        sv::sleep(deltaTime);
    }

    EXPECT_TRUE(client.getState() == sv::net::ConnectionState::Enum::Connected);
    EXPECT_TRUE(server.getState() == sv::net::ConnectionState::Enum::Connected);

    // Introduce another connection and verify that it fails (busy)
    sv::net::Connection busy(protocolId, timeout);
    EXPECT_TRUE(busy.start(clientPort + 1));
    busy.connect(sv::net::Address(127, 0, 0, 1, serverPort));

    while (true) {
        if (busy.getState() != sv::net::ConnectionState::Enum::Connecting ||
            busy.getState() == sv::net::ConnectionState::Enum::Connected) {
            break;
        }

        uint8_t clientPacket[] = "client to server";
        client.sendPacket(clientPacket, sizeof(clientPacket));

        uint8_t serverPacket[] = "server to client";
        server.sendPacket(serverPacket, sizeof(serverPacket));

        uint8_t busyPacket[] = "i'm busy!";
        busy.sendPacket(busyPacket, sizeof(busyPacket));

        while (true) {
            uint8_t packet[256];
            size_t bytesRead = client.receivePacket(packet, sizeof(packet));
            if (bytesRead == 0) {
                break;
            }
        }

        while (true) {
            uint8_t packet[256];
            size_t bytesRead = server.receivePacket(packet, sizeof(packet));
            if (bytesRead == 0) {
                break;
            }
        }

        while (true) {
            uint8_t packet[256];
            size_t bytesRead = busy.receivePacket(packet, sizeof(packet));
            if (bytesRead == 0) {
                break;
            }
        }

        client.update(deltaTime);
        server.update(deltaTime);
        busy.update(deltaTime);

        sv::sleep(deltaTime);
    }

    EXPECT_TRUE(client.getState() == sv::net::ConnectionState::Enum::Connected);
    EXPECT_TRUE(server.getState() == sv::net::ConnectionState::Enum::Connected);
    EXPECT_TRUE(busy.getState() != sv::net::ConnectionState::Enum::Connected);
    EXPECT_TRUE(busy.getState() == sv::net::ConnectionState::Enum::ConnectFail);

    sv::net::shutdownSockets();
}

TEST(Connect, TestRejoin) {
    EXPECT_TRUE(sv::net::initializeSockets());

    const int16_t serverPort = 30000;
    const int16_t clientPort = 30001;
    const int32_t protocolId = 0x11112222;
    const float deltaTime    = 0.001f;
    const float timeout      = 0.1f;

    sv::net::Connection client(protocolId, timeout);
    sv::net::Connection server(protocolId, timeout);

    EXPECT_TRUE(client.start(clientPort));
    EXPECT_TRUE(server.start(serverPort));

    client.connect(sv::net::Address(127, 0, 0, 1, serverPort));
    server.listen();

    // Establish connection between server and client
    while (true) {
        if (client.getState() == sv::net::ConnectionState::Enum::Connected &&
            server.getState() == sv::net::ConnectionState::Enum::Connected) {
            break;
        }

        if (client.getState() != sv::net::ConnectionState::Enum::Connecting &&
            client.getState() == sv::net::ConnectionState::Enum::ConnectFail) {
            break;
        }

        uint8_t clientPacket[] = "client to server";
        client.sendPacket(clientPacket, sizeof(clientPacket));

        uint8_t serverPacket[] = "server to client";
        server.sendPacket(serverPacket, sizeof(serverPacket));

        while (true) {
            uint8_t packet[256];
            size_t bytesRead = client.receivePacket(packet, sizeof(packet));
            if (bytesRead == 0) {
                break;
            }
        }

        while (true) {
            uint8_t packet[256];
            size_t bytesRead = server.receivePacket(packet, sizeof(packet));
            if (bytesRead == 0) {
                break;
            }
        }

        client.update(deltaTime);
        server.update(deltaTime);

        sv::sleep(deltaTime);
    }

    EXPECT_TRUE(client.getState() == sv::net::ConnectionState::Enum::Connected);
    EXPECT_TRUE(server.getState() == sv::net::ConnectionState::Enum::Connected);

    // Let connection timeout

    while (client.getState() == sv::net::ConnectionState::Enum::Connected ||
           server.getState() == sv::net::ConnectionState::Enum::Connected) {
        while (true) {
            uint8_t packet[256];
            size_t bytesRead = client.receivePacket(packet, sizeof(packet));
            if (bytesRead == 0) {
                break;
            }
        }

        while (true) {
            uint8_t packet[256];
            size_t bytesRead = server.receivePacket(packet, sizeof(packet));
            if (bytesRead == 0) {
                break;
            }
        }

        client.update(deltaTime);
        server.update(deltaTime);

        sv::sleep(deltaTime);
    }

    EXPECT_FALSE(client.getState() ==
                 sv::net::ConnectionState::Enum::Connected);
    EXPECT_FALSE(server.getState() ==
                 sv::net::ConnectionState::Enum::Connected);

    // Reconnect client

    client.connect(sv::net::Address(127, 0, 0, 1, serverPort));

    while (true) {
        if (client.getState() == sv::net::ConnectionState::Enum::Connected &&
            server.getState() == sv::net::ConnectionState::Enum::Connected) {
            break;
        }

        if (client.getState() != sv::net::ConnectionState::Enum::Connecting &&
            client.getState() == sv::net::ConnectionState::Enum::ConnectFail) {
            break;
        }

        uint8_t clientPacket[] = "client to server";
        client.sendPacket(clientPacket, sizeof(clientPacket));

        uint8_t serverPacket[] = "server to client";
        server.sendPacket(serverPacket, sizeof(serverPacket));

        while (true) {
            uint8_t packet[256];
            size_t bytesRead = client.receivePacket(packet, sizeof(packet));
            if (bytesRead == 0) {
                break;
            }
        }

        while (true) {
            uint8_t packet[256];
            size_t bytesRead = server.receivePacket(packet, sizeof(packet));
            if (bytesRead == 0) {
                break;
            }
        }

        client.update(deltaTime);
        server.update(deltaTime);

        sv::sleep(deltaTime);
    }

    EXPECT_TRUE(client.getState() == sv::net::ConnectionState::Enum::Connected);
    EXPECT_TRUE(server.getState() == sv::net::ConnectionState::Enum::Connected);

    sv::net::shutdownSockets();
}
