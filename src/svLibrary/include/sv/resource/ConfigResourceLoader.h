//===-- sv/resource/ConfigResourceLoader.h - Load .cfg files ----*- C++ -*-===//
//
//                 The Special Engine Variant Game Engine
//
// This file is distributed under the MIT License. See LICENSE.txt for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Extra processing required to load config files from a resource cache.
///
//===----------------------------------------------------------------------===//
#pragma once

#include <sv/resource/ResourceCache.h>

namespace sv {
class ConfigResourceLoader : public ResourceLoader {
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
}
