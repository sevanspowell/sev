#include <cassert>
#include <memory>

#include "sv/client/Client.h"

TEST(Client, Initialize) {
    sv::Client client;

    EXPECT_TRUE(client.initialize(sv::ProgramOptions(0, nullptr)));
}
