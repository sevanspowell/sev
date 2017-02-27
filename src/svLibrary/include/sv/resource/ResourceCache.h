//===-- sv/resource/ResourceCache.h - Resource cache ------------*- C++ -*-===//
//
//                 The Special Engine Variant Game Engine
//
// This file is distributed under the MIT License. See LICENSE.txt for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Keep track of recently used Resources, keep resources in memory until
/// they are no longer being used.
///
/// The resource cache holds a list of resource collections, when a user of the
/// class requests a resource, the most recently modified resource with that
/// identifier is found in the resource collections.
///
/// When asked to open a resource, the resource cache provides a handle to that
/// resource (ResourceHandle).
///
/// Particular resources may require extra processing after loading before they
/// can be used. The 'ResourceExtraData' class provides a way for us to retrieve
/// this data from a ResourceHandle.
///
/// Based off of the resource system in 'Game Coding Complete' by Mike McShaffry
/// and David Graham.
///
//===----------------------------------------------------------------------===//
#pragma once

#include <list>
#include <map>
#include <memory>
#include <vector>

#include <sv/resource/Resource.h>
#include <sv/resource/ResourceCollection.h>

namespace sv {
///-----------------------------------------------------------------------------
/// Interface thru which to access extra data for resources that are processed
/// after they are loaded
///-----------------------------------------------------------------------------
class ResourceExtraData {
    virtual ~ResourceExtraData() {}
};

class ResourceHandle;

///-----------------------------------------------------------------------------
/// A resource loader is used to perform extra processing on a loaded file (if
/// necessary).
///-----------------------------------------------------------------------------
class ResourceLoader {
  public:
    virtual ~ResourceLoader() {}

    ///-------------------------------------------------------------------------
    /// Get a wildcard pattern representing the types of files this resource
    /// loader should be used for (e.g. *.wav, *.png, *)
    ///-------------------------------------------------------------------------
    virtual std::string getPattern() const = 0;

    ///-------------------------------------------------------------------------
    /// \returns True, if this resource loader doesn't need to do anything,
    /// false otherwise.
    ///-------------------------------------------------------------------------
    virtual bool useRawFile() const = 0;

    ///-------------------------------------------------------------------------
    /// Assuming this loader does some processing on a loaded file, should the
    /// original loaded file buffer be discarded after that processing is done?
    ///
    /// \returns True if original buffer can be discarded, false otherwise.
    ///-------------------------------------------------------------------------
    virtual bool discardRawBufferAfterLoad() const = 0;

    ///-------------------------------------------------------------------------
    /// Given a raw buffer, get the size of the loaded resource data after
    /// processing.
    ///-------------------------------------------------------------------------
    virtual size_t getLoadedResourceSize(const void *rawBuffer,
                                         size_t rawBufferSize) const = 0;

    ///-------------------------------------------------------------------------
    /// Perform extra processing and put the loaded data in the given resource
    /// handle.
    ///
    /// \pre \p rawBuffer guaranteed not be nullptr.
    /// \pre \p handle will have enough memory in it's buffer for the loaded
    /// resource (according to getLoadedResourceSize).
    ///-------------------------------------------------------------------------
    virtual bool loadResource(const void *rawBuffer, size_t rawSize,
                              std::shared_ptr<ResourceHandle> &handle) = 0;
};

class DefaultResourceLoader : public ResourceLoader {
  public:
    /// \copydoc ResourceLoader::getPattern
    virtual std::string getPattern() const;

    /// \copydoc ResourceLoader::useRawFile
    virtual bool useRawFile() const;

    /// \copydoc ResourceLoader::discardRawBufferAfterLoad
    virtual bool discardRawBufferAfterLoad() const;

    /// \copydoc ResourceLoader::getLoadedResourceSize
    virtual size_t getLoadedResourceSize(const void *rawBuffer,
                                         size_t rawBufferSize) const;

    /// \copydoc ResourceLoader::loadResource
    virtual bool loadResource(const void *rawBuffer, size_t rawSize,
                              std::shared_ptr<ResourceHandle> &handle);
};

class ResourceCache;

/// Handle to a resource.
class ResourceHandle {
    friend class ResourceCache;

  public:
    ResourceHandle(const Resource &resource_, void *rawBuffer_,
                   size_t rawBufferSize_, ResourceCache &resourceCache_)
        : resource(resource_), rawBuffer(rawBuffer_),
          rawBufferSize(rawBufferSize_), resourceCache(resourceCache_) {}

    ~ResourceHandle();

    /// Get resource this resource handle refers to.
    const Resource &getResource() const;

    /// Get the size of the buffer holding this resource's data.
    size_t getResourceSize() const;

    /// Get a pointer to a read-only buffer of the resource's data.
    const void *getResourceBuffer() const;

    /// Get a pointer to a mutable buffer of the resource's data.
    void *getMutableResourceBuffer();

    /// Get a pointer to extra data that may be associated with the resource.
    std::shared_ptr<ResourceExtraData> getExtraData();

    /// Set the extra data associated with this resource.
    void setExtraData(const std::shared_ptr<ResourceExtraData> &data);

  private:
    Resource resource;
    void *rawBuffer;
    size_t rawBufferSize;
    std::shared_ptr<ResourceExtraData> extraData;
    ResourceCache &resourceCache;
};

class ResourceCache {
    friend class ResourceHandle;

  public:
    ///-------------------------------------------------------------------------
    /// Initialize the resource cache with a given amount of memory.
    ///-------------------------------------------------------------------------
    ResourceCache(const size_t sizeInMb);

    ///-------------------------------------------------------------------------
    /// \returns True if initialization successful, false otherwise.
    ///-------------------------------------------------------------------------
    bool initialize();

    ///-------------------------------------------------------------------------
    /// Add a resource collection to the resource cache.
    ///
    /// \returns True if resource collection was added and opened successfully,
    /// false otherwise.
    ///-------------------------------------------------------------------------
    bool registerResourceCollection(
        const std::shared_ptr<ResourceCollection> &resourceCollection);

    ///-------------------------------------------------------------------------
    /// Add a resource loader to the resource cache.
    ///
    /// Resource loaders added later will be given a higher priority than
    /// loaders added earlier.
    ///-------------------------------------------------------------------------
    void registerResourceLoader(
        const std::shared_ptr<ResourceLoader> &resourceLoader);

    ///-------------------------------------------------------------------------
    /// Get a handle to a resource, loading it if necessary.
    ///-------------------------------------------------------------------------
    std::shared_ptr<ResourceHandle> getHandle(const Resource &resource);

    ///-------------------------------------------------------------------------
    /// Free every handle in the resource cache.
    ///-------------------------------------------------------------------------
    void flush();

  private:
    ///-------------------------------------------------------------------------
    /// Tries to make room in the cache for \p size bytes.
    ///
    /// Will free other resources to make room if necessary
    ///
    /// \returns True if able to make room, false otherwise.
    ///-------------------------------------------------------------------------
    bool makeRoom(size_t size);

    ///-------------------------------------------------------------------------
    /// Allocate \p size bytes of memory.
    ///
    /// Will free other resources to make room if necessary.
    ///-------------------------------------------------------------------------
    void *allocate(size_t size);

    ///-------------------------------------------------------------------------
    /// Remove the given resource from the cache.
    ///-------------------------------------------------------------------------
    void freeHandle(const std::shared_ptr<ResourceHandle> &handle);

    ///-------------------------------------------------------------------------
    /// Load a given resource
    ///-------------------------------------------------------------------------
    std::shared_ptr<ResourceHandle> load(const Resource &resource);

    ///-------------------------------------------------------------------------
    /// Find a resource in the cache
    ///-------------------------------------------------------------------------
    std::shared_ptr<ResourceHandle> find(const Resource &resource);

    ///-------------------------------------------------------------------------
    /// Move the given resource handle to the front of the least-recently used
    /// list, indicating that it's been used recently.
    ///-------------------------------------------------------------------------
    void update(const std::shared_ptr<ResourceHandle> &handle);

    ///-------------------------------------------------------------------------
    /// Free the least recently used resource.
    ///-------------------------------------------------------------------------
    void freeOneResource();

    ///-------------------------------------------------------------------------
    /// Called whenever memory associated with a resource is actually freed.
    ///-------------------------------------------------------------------------
    void memoryHasBeenFreed(size_t size);

    typedef std::vector<std::shared_ptr<ResourceCollection>>
        ResourceCollections;
    typedef std::vector<std::shared_ptr<ResourceLoader>> ResourceLoaders;
    typedef std::map<Resource, std::shared_ptr<ResourceHandle>>
        ResourceHandleMap;
    typedef std::list<std::shared_ptr<ResourceHandle>> ResourceHandleList;

    ResourceCollections resourceCollections;
    ResourceLoaders resourceLoaders;
    ResourceHandleMap resources;
    ResourceHandleList leastRecentlyUsedList;

    // Max size of resource cache in bytes
    size_t cacheSize;
    // Amount used in bytes
    size_t allocated;
};
}
