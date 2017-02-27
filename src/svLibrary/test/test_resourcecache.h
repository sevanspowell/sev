#include <iostream>

#include <sv/console/ConsoleCommands.h>
#include <sv/resource/ConfigResourceLoader.h>
#include <sv/resource/ResourceCache.h>

namespace resource_cache {
const std::string assetDir("./src/svLibrary/test/assets/test_resourcecache");
}

TEST(ResourceCache, OpenFile) {
    sv::ResourceCache cache(1);

    EXPECT_TRUE(cache.initialize());
    EXPECT_TRUE(
        cache.registerResourceCollection(std::shared_ptr<sv::ResourceFolderPC>(
            new sv::ResourceFolderPC(resource_cache::assetDir))));

    sv::Resource file("0helloworld.txt");
    std::shared_ptr<sv::ResourceHandle> handle = cache.getHandle(file);
    EXPECT_TRUE(handle != nullptr);

    std::shared_ptr<sv::ResourceHandle> handle2 = cache.getHandle(file);
    EXPECT_TRUE(handle->getResourceSize() == handle2->getResourceSize());
    EXPECT_TRUE(handle->getResource().name == handle2->getResource().name);
    // Should be same, re-used buffer
    EXPECT_TRUE(handle->getResourceBuffer() == handle2->getResourceBuffer());
}

TEST(ResourceCache, ExecCommand) {
    sv::ResourceCache cache(1);

    EXPECT_TRUE(cache.initialize());
    EXPECT_TRUE(
        cache.registerResourceCollection(std::shared_ptr<sv::ResourceFolderPC>(
            new sv::ResourceFolderPC(resource_cache::assetDir))));
    cache.registerResourceLoader(std::shared_ptr<sv::ConfigResourceLoader>(
        new sv::ConfigResourceLoader()));

    sv::Console console;
    std::shared_ptr<sv::ExecCommand> execCmd(new sv::ExecCommand(cache));
    std::shared_ptr<sv::EchoCommand> echoCmd(new sv::EchoCommand());

    console.registerCommand("exec", execCmd);
    console.registerCommand("echo", echoCmd);
    EXPECT_TRUE(console.executeString("exec test.cfg"));
    EXPECT_EQ(console.getOutputBuffer(),
              std::string("\"Hello world\";\n\"No\";\n"));
    EXPECT_EQ(console.getErrorBuffer().size(), 0);
}
