#include <sv/resource/ResourceCache.h>

namespace resource_cache {
const std::string assetDir("./src/svLibrary/test/assets");
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
