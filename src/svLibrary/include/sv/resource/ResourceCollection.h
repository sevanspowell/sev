//===-- sv/resource/ResourceCollection.h - bundle of resources --*- C++ -*-===//
//
//                 The Special Engine Variant Game Engine
//
// This file is distributed under the MIT License. See LICENSE.txt for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief A collection of resources (usually a resource file [.zip] or folder).
///
/// The Resource collection is an interface used to represent some collection of
/// resources. It allows users of the class to access the resources stored in
/// the collection.
///
/// The Resource class is used to uniquely identify resources.
///
//===----------------------------------------------------------------------===//
#pragma once

#include <sv/resource/DateTime.h>
#include <sv/resource/Resource.h>

namespace sv {
///-----------------------------------------------------------------------------
/// This class represents a collection of resources stored in some form and
/// allows users of the class to access the resources stored in the collection
/// and relevant information.
///
/// The Resource class is used to uniquely identify resources.
///-----------------------------------------------------------------------------
class IResourceCollection {
  public:
    ///-------------------------------------------------------------------------
    /// Virtual destructor.
    ///-------------------------------------------------------------------------
    virtual ~IResourceCollection() {}
    ///-------------------------------------------------------------------------
    /// Open the resource file for reading.
    ///
    /// \returns True if opening the file was successful and false otherwise.
    ///-------------------------------------------------------------------------
    virtual bool open() = 0;

    ///-------------------------------------------------------------------------
    /// Get the uncompressed size (in bytes) of the given resource in the
    /// implementing resource file.
    ///
    /// \param   Resource to get the size of.
    /// \returns Size of the given resource file in bytes or -1 if resource file
    /// not found.
    ///-------------------------------------------------------------------------
    virtual int32_t getRawResourceSize(const Resource &r) = 0;

    ///-------------------------------------------------------------------------
    /// Get a buffer containing the raw data of the given resource in the
    /// implementing resource file.
    ///
    /// \pre Buffer must be pre-allocated with the at least enough memory to
    /// hold the data associated with the given resource file.
    ///
    /// \param   Resource to get the data for.
    /// \param   Pointer to buffer to fill with resource data.
    /// \returns Size of the raw data, or -1 in case of error.
    ///-------------------------------------------------------------------------
    virtual int32_t getRawResource(const Resource &r, void *const buffer) = 0;

    ///-------------------------------------------------------------------------
    /// Get the total number of resources in the resource file.
    ///
    /// \returns Number of resources in the resource file.
    ///-------------------------------------------------------------------------
    virtual size_t getNumResources() const = 0;

    ///-------------------------------------------------------------------------
    /// Get the unique resource identifier for the resource file at the given
    /// index in the resource collection.
    ///
    /// \param   Index of resource in the resource collection.
    /// \returns Unique resource identifier.
    ///-------------------------------------------------------------------------
    virtual Resource getResourceIdentifier(size_t index) const = 0;

    ///-------------------------------------------------------------------------
    /// Get the last modified date of the given resource in the resource
    /// collection.
    ///
    /// \param   Unique resource identifier.
    /// \returns Structure describing the last modified date of the given
    /// resource.
    ///-------------------------------------------------------------------------
    virtual DateTime getResourceModifiedDate(const Resource &r) const = 0;
};
}
