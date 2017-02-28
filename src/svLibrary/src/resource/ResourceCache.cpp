#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <sstream>

#include <sv/Globals.h>
#include <sv/Common.h>
#include <sv/resource/ResourceCache.h>

namespace sv {
std::string DefaultResourceLoader::getPattern() const { return "*"; }

bool DefaultResourceLoader::useRawFile() const { return true; }

bool DefaultResourceLoader::discardRawBufferAfterLoad() const { return false; }

size_t
DefaultResourceLoader::getLoadedResourceSize(const void *rawBuffer,
                                             size_t rawBufferSize) const {
    return rawBufferSize;
}

bool DefaultResourceLoader::loadResource(
    const void *rawBuffer, size_t rawSize,
    std::shared_ptr<ResourceHandle> &handle) {
    return true;
}

ResourceHandle::~ResourceHandle() {
    free(rawBuffer);
    resourceCache.memoryHasBeenFreed(rawBufferSize);
}

const Resource &ResourceHandle::getResource() const { return resource; }

size_t ResourceHandle::getResourceSize() const { return rawBufferSize; }

const void *ResourceHandle::getResourceBuffer() const { return rawBuffer; }

void *ResourceHandle::getMutableResourceBuffer() { return rawBuffer; }

std::shared_ptr<ResourceExtraData> ResourceHandle::getExtraData() {
    return extraData;
}

void ResourceHandle::setExtraData(
    const std::shared_ptr<ResourceExtraData> &data) {
    extraData = data;
}

ResourceCache::ResourceCache(const size_t sizeInMb) {
    cacheSize = sizeInMb * 1024 * 1024; // Conver megabytes to bytes
    allocated = 0;
}

bool ResourceCache::initialize() {
    registerResourceLoader(
        std::shared_ptr<ResourceLoader>(new DefaultResourceLoader()));

    return true;
}

bool ResourceCache::registerResourceCollection(
    const std::shared_ptr<ResourceCollection> &resourceCollection) {
    bool result = false;

    if (resourceCollection->isOpen() == false) {
        result = resourceCollection->open();
    }

    if (result == true) {
        resourceCollections.push_back(resourceCollection);
    }
    
    return result;
}

void ResourceCache::registerResourceLoader(
    const std::shared_ptr<ResourceLoader> &resourceLoader) {
    resourceLoaders.push_back(resourceLoader);
}

std::shared_ptr<ResourceHandle>
ResourceCache::getHandle(const Resource &resource) {
    std::shared_ptr<ResourceHandle> handle((find(resource)));

    if (handle == nullptr) {
        handle = load(resource);
    } else {
        update(handle);
    }

    return handle;
}

void ResourceCache::flush() {
    while (!leastRecentlyUsedList.empty()) {
        std::shared_ptr<ResourceHandle> handle =
            *(leastRecentlyUsedList.begin());
        freeHandle(handle);
        leastRecentlyUsedList.pop_front();
    }
}

bool ResourceCache::makeRoom(size_t size) {
    if (size > cacheSize) {
        return false;
    }

    // Return false if there's no possible way to allocate the memory
    while (size > (cacheSize - allocated)) {
        // The cache is empty, and there's still not enough room.
        if (leastRecentlyUsedList.empty()) {
            return false;
        }

        freeOneResource();
    }

    return true;
}

void *ResourceCache::allocate(size_t size) {
    if (!makeRoom(size)) {
        return nullptr;
    }

    void *mem = malloc(size);
    if (mem != nullptr) {
        allocated += size;
    }

    return mem;
}

void ResourceCache::freeHandle(const std::shared_ptr<ResourceHandle> &handle) {
    leastRecentlyUsedList.remove(handle);
    resources.erase(handle->resource);
}

std::shared_ptr<ResourceHandle> ResourceCache::load(const Resource &resource) {
    // Create a new resource and add it to the least-recently used list and map.
    std::shared_ptr<ResourceLoader> loader;
    std::shared_ptr<ResourceHandle> handle;

    // Find resource loader to use
    for (ResourceLoaders::reverse_iterator it = resourceLoaders.rbegin();
         it != resourceLoaders.rend(); ++it) {
        std::shared_ptr<ResourceLoader> testLoader = *it;

        if (wildcardMatch(testLoader->getPattern().c_str(),
                          resource.name.c_str())) {
            loader = testLoader;
            break;
        }
    }

    // No loader found to load this type of resource
    if (loader == nullptr) {
        assert(loader &&
               "Resource loader not found for this type of resource!");
        return handle;
    }

    // Find resource collection containing most recent version of resource
    std::shared_ptr<ResourceCollection> collection;
    DateTime latest(0, 0, 0, 0, 0, 0);

    for (ResourceCollections::iterator it = resourceCollections.begin();
         it != resourceCollections.end(); ++it) {
        std::shared_ptr<ResourceCollection> testCollection = *it;

        // If resource found
        if (testCollection->getRawResourceSize(resource) >= 0) {
            DateTime modified =
                testCollection->getResourceModifiedDate(resource);
            // If newer resource
            if (modified > latest) {
                // Use this collection unless we find a newer one
                collection = testCollection;
                // Update latest datetime
                latest = modified;
            }
        }
    }

    // Resource file not found in any collection
    if (collection == nullptr) {
        std::stringstream err;
        err << "'" << resource.name
            << "' not found in any resource collection.";
        globals::log(LogArea::Enum::Common, LogLevel::Enum::Warning,
                        err.str());
        return handle;
    }

    size_t rawSize  = collection->getRawResourceSize(resource);
    void *rawBuffer = allocate(rawSize);
    if (rawBuffer == nullptr) {
        return handle;
    }
    memset(rawBuffer, 0, rawSize);

    // Resource cache has run out of memory or couldn't load raw resource
    if (collection->getRawResource(resource, rawBuffer) == -1) {
        return handle;
    }

    void *buffer      = nullptr;
    size_t bufferSize = 0;

    if (loader->useRawFile()) {
        buffer = rawBuffer;
        handle = std::shared_ptr<ResourceHandle>(
            new ResourceHandle(resource, buffer, rawSize, *this));
    } else {
        bufferSize = loader->getLoadedResourceSize(rawBuffer, rawSize);
        buffer     = allocate(bufferSize);

        // Resource cache out of memory
        if (rawBuffer == nullptr || buffer == nullptr) {
            return handle;
        }

        handle = std::shared_ptr<ResourceHandle>(
            new ResourceHandle(resource, buffer, bufferSize, *this));

        bool success = loader->loadResource(rawBuffer, rawSize, handle);

        if (loader->discardRawBufferAfterLoad()) {
            free(rawBuffer);
        }

        if (!success) {
            return std::shared_ptr<ResourceHandle>();
        }
    }

    // Everything worked
    // Insert resource into resource handle map
    resources[resource] = handle;

    return handle;
}

std::shared_ptr<ResourceHandle> ResourceCache::find(const Resource &resource) {
    ResourceHandleMap::iterator it = resources.find(resource.name);
    if (it == resources.end()) {
        return std::shared_ptr<ResourceHandle>();
    }

    return it->second;
}

void ResourceCache::update(const std::shared_ptr<ResourceHandle> &handle) {
    leastRecentlyUsedList.remove(handle);
    leastRecentlyUsedList.push_front(handle);
}

void ResourceCache::freeOneResource() {
    // Get last element
    ResourceHandleList::iterator toRemove = leastRecentlyUsedList.end();
    toRemove--;

    std::shared_ptr<ResourceHandle> handle = *toRemove;

    leastRecentlyUsedList.pop_back();
    resources.erase(handle->resource.name);
}

void ResourceCache::memoryHasBeenFreed(size_t size) { allocated -= size; }
}
