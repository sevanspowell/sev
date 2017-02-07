//===-- sv/platform/SDL2Platform.h - SDL2 Platform impl. --------*- C++ -*-===//
//
//                 The Special Engine Variant Game Engine
//
// This file is distributed under the MIT License. See LICENSE.txt for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief SDL2 Operating System abstraction implementation.
///
//===----------------------------------------------------------------------===//
#pragma once

#include <sv/platform/Platform.h>

namespace sv {
class SDL2Platform : public Platform {
  public:
    /// \copydoc Platform::initialize()
    virtual bool initialize();

    /// \copydoc Platform::shutdown()
    virtual void shutdown();

    /// \copydoc Platform::getMilliseconds()
    virtual uint32_t getMilliseconds() const;

    /// \copydoc Platform::getNextEvent()
    virtual PlatformEvent getNextEvent();
};
}
