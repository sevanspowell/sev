#include <cstring>

#include <sv/resource/ResourceFolderPC.h>

namespace resource_folder {
const std::string assetDir("./src/svLibrary/test/assets/test_resourcefolderpc");
const size_t numAssets = 1;
}

TEST(ResourceFolderPC, OpenFolder) {
    sv::ResourceFolderPC folder(resource_folder::assetDir);

    EXPECT_FALSE(folder.isOpen());
    EXPECT_TRUE(folder.open());
    EXPECT_TRUE(folder.isOpen());
}

TEST(ResourceFolderPC, OpenFile) {
    sv::ResourceFolderPC folder(resource_folder::assetDir);

    EXPECT_TRUE(folder.open());
    sv::Resource file("0helloworld.txt");

    int32_t size = folder.getRawResourceSize(file);
    EXPECT_EQ(13, size);

    char *buffer = (char *)malloc(size);
    memset(buffer, '\0', size);

    EXPECT_EQ(size, folder.getRawResource(file, (void *)buffer));

    EXPECT_TRUE(strncmp(buffer, "Hello world!\n", size) == 0);

    free(buffer);
}

TEST(ResourceFolderPC, GetNumResources) {
    sv::ResourceFolderPC folder(resource_folder::assetDir);

    EXPECT_TRUE(folder.open());

    EXPECT_EQ(resource_folder::numAssets, folder.getNumResources());

    sv::ResourceFolderPC folder2("");

    EXPECT_TRUE(folder.open());

    EXPECT_EQ(0, folder2.getNumResources());
}

TEST(ResourceFolderPC, GetResourceByIndex) {
    sv::ResourceFolderPC folder(resource_folder::assetDir);

    EXPECT_TRUE(folder.open());
    sv::Resource file("0helloworld.txt");

    EXPECT_EQ(file.name, folder.getResourceIdentifier(0).name);
    EXPECT_EQ(std::string(""), folder.getResourceIdentifier(1).name);
}

TEST(ResourceFolderPC, GetResourceModifiedDate) {
    sv::ResourceFolderPC folder(resource_folder::assetDir);

    EXPECT_TRUE(folder.open());
    sv::Resource file("0helloworld.txt");

    sv::DateTime modifiedLast(16, 33, 14, 24, 1, 2017);

    EXPECT_TRUE(modifiedLast == folder.getResourceModifiedDate(file));
}
