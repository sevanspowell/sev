//===-- sv/resource/Resource.h - Resource identifier ------------*- C++ -*-===//
//
//                 The Special Engine Variant Game Engine
//
// This file is distributed under the MIT License. See LICENSE.txt for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief The Resource class is used to uniquely identify a resource.
///
//===----------------------------------------------------------------------===//
#pragma once

#include <algorithm>
#include <string>

namespace sv {
///-----------------------------------------------------------------------------
/// The Resouce class is used to uniquely identity a resource in a
/// ResourceCollection.
///-----------------------------------------------------------------------------
class Resource {
  public:
    Resource(const std::string &name_) : name(name_) {
        // Force resource names to be lower case
        std::transform(name.begin(), name.end(), name.begin(),
                       (int (*)(int))std::tolower);
    }

    std::string name;

    // TODO Is Binary or Intermediate resource?
};

bool operator<(const Resource &r1, const Resource &r2);
}
