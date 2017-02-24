//===-- sv/resource/ResourceFolderPC.h - Resource folder for PC -*- C++ -*-===//
//
//                 The Special Engine Variant Game Engine
//
// This file is distributed under the MIT License. See LICENSE.txt for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Resource collection implementation for plain folders on Windows,
/// Mac and Linux platforms.
///
//===----------------------------------------------------------------------===//
#pragma once

#include <sv/resource/ResourceCollection.h>

namespace sv {
///-----------------------------------------------------------------------------
/// Resource collection for resource stored in a folder on Windows, Mac and
/// Linux platforms.
///
/// We assume that all resource identifiers are relative to the folder path.
///-----------------------------------------------------------------------------
class ResourceFolderPC : public IResourceCollection {
  public:
    ResourceFolderPC(const std::string &folderPath_)
        : folderPath(folderPath_) {}

    /// \copydoc ResourceCollection::open
    virtual bool open();

    /// \copydoc ResourceCollection:getRawResourceSize
    virtual int32_t getRawResourceSize(const Resource &r);

    /// \copydoc ResourceCollection::getRawResource
    virtual int32_t getRawResource(const Resource &r, void *const buffer);

    /// \copydoc ResourceCollection::getNumResources
    virtual size_t getNumResources() const;

    /// \copydoc ResourceCollection::getResourceIdentifier
    virtual Resource getResourceIdentifier(size_t index) const;

    /// \copydoc ResourceCollection::getResourceModifiedDate
    virtual DateTime getResourceModifiedDate(const Resource &r) const;

  private:
    ///-------------------------------------------------------------------------
    /// Get the number of files in a given directory.
    ///
    /// \param Path to directory to get number of files in.
    /// \param Number of files in directory.
    ///-------------------------------------------------------------------------
    size_t getNumFilesInDir(const std::string &dirPath) const;

    ///-------------------------------------------------------------------------
    /// Get the path to the file (relative to dirPath) in the given directory at
    /// the given index (note that this method works recursively, so folders
    /// within the dirPath will be included in the search and not counted as
    /// files themselves).
    ///
    /// \param   Path to directory to search.
    /// \param   Index of file to get.
    /// \returns Path to file at the given index (relative to dirPath), or empty
    /// string if no file at the given index.
    ///-------------------------------------------------------------------------
    std::string getFileNameInDirByIndex(const std::string &dirPath,
                                        size_t indexToFind) const;

    /// Recursive helper function for getFileNameInDirByIndex.
    std::string getFileNameInDirByIndex(const std::string &dirPath,
                                        size_t indexToFind,
                                        size_t &currentIndex) const;

    std::string folderPath;
};
}
