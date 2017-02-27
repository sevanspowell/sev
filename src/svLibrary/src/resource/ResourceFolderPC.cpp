#include <sv/System.h>

#if SV_PLATFORM_POSIX
#include <cstdio>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif

#include <fstream>
#include <sstream>

#include <sv/resource/ResourceFolderPC.h>

namespace sv {
bool ResourceFolderPC::open() {
    isFolderOpen = true;
    // No need to 'open' a folder
    return true;
}

bool ResourceFolderPC::isOpen() { return isFolderOpen; }

int32_t ResourceFolderPC::getRawResourceSize(const Resource &r) {
    std::stringstream fullPath;
    fullPath << folderPath << "/" << r.name;

    std::ifstream in(fullPath.str(),
                     std::ifstream::ate | std::ifstream::binary);

    return in.tellg();
}

int32_t ResourceFolderPC::getRawResource(const Resource &r,
                                         void *const buffer) {
    // http://stackoverflow.com/questions/18816126/c-read-the-whole-file-in-buffer
    std::stringstream fullPath;
    fullPath << folderPath << "/" << r.name;

    std::ifstream in(fullPath.str(),
                     std::ifstream::ate | std::ifstream::binary);
    std::streamsize size = in.tellg();
    in.seekg(0, std::ios::beg);

    if (in.read((char *const)buffer, size)) {
        // Successful
        return size;
    } else {
        return -1;
    }
}

size_t ResourceFolderPC::getNumResources() const {
    return getNumFilesInDir(folderPath);
}

Resource ResourceFolderPC::getResourceIdentifier(size_t index) const {
    size_t i = 0;
    std::string resourceIdentifier =
        getFileNameInDirByIndex(folderPath, index, i);

    return Resource(resourceIdentifier);
}

DateTime ResourceFolderPC::getResourceModifiedDate(const Resource &r) const {
    DateTime dateTime;

#if SV_PLATFORM_POSIX
    struct tm *clock;
    struct stat attr;
    std::stringstream filePath;
    filePath << folderPath << "/" << r.name;

    stat(filePath.str().c_str(), &attr);
    clock = localtime(&(attr.st_mtime));

    dateTime = DateTime(clock->tm_sec, clock->tm_min, clock->tm_hour,
                        clock->tm_mday, clock->tm_mon, clock->tm_year + 1900);

#else
// TODO Windows
#endif

    return dateTime;
}

size_t ResourceFolderPC::getNumFilesInDir(const std::string &dirPath) const {
    size_t numFiles = 0;

#if SV_PLATFORM_POSIX
    DIR *dp;
    struct dirent *ep = nullptr;
    dp                = opendir(dirPath.c_str());

    if (dp != nullptr) {
        while ((ep = readdir(dp))) {
            // Ignore ., .. and all other hidden files
            if (strncmp(ep->d_name, ".", 1) != 0) {
                if (ep->d_type == DT_REG) {

                    ++numFiles;
                } else if (ep->d_type == DT_DIR) {
                    std::stringstream newDir;
                    newDir << dirPath << "/" << std::string(ep->d_name);
                    numFiles += getNumFilesInDir(newDir.str());
                }
            }
        }

        closedir(dp);
    }
#else
// TODO Windows
#endif

    return numFiles;
}

std::string
ResourceFolderPC::getFileNameInDirByIndex(const std::string &dirPath,
                                          size_t indexToFind) const {
    size_t currentIndex = 0;
    return getFileNameInDirByIndex(dirPath, indexToFind, currentIndex);
}

std::string
ResourceFolderPC::getFileNameInDirByIndex(const std::string &dirPath,
                                          size_t indexToFind,
                                          size_t &currentIndex) const {
    std::string resourceName = "";

#if SV_PLATFORM_POSIX
    DIR *dp;
    struct dirent *ep = nullptr;
    dp                = opendir(dirPath.c_str());

    if (dp != nullptr) {
        while ((ep = readdir(dp))) {
            // Ignore ., .. and all other hidden files
            if (strncmp(ep->d_name, ".", 1) != 0) {
                if (ep->d_type == DT_REG) {
                    // For each regular file, check if index we are looking
                    // for, else increase current index
                    if (currentIndex == indexToFind) {
                        resourceName = ep->d_name;
                        break;
                    }
                    ++currentIndex;
                } else if (ep->d_type == DT_DIR) {
                    std::stringstream newDir;
                    newDir << dirPath << "/" << std::string(ep->d_name);
                    resourceName = getFileNameInDirByIndex(
                        newDir.str(), indexToFind, currentIndex);
                    if (resourceName != "") {
                        break;
                    }
                }
            }
        }

        closedir(dp);
    }
#else
// TODO Windows
#endif

    return resourceName;
}
}
