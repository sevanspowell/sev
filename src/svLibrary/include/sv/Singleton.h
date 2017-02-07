//===-- sv/Singleton.h - Singleton base class -------------------*- C++ -*-===//
//
//                 The Special Engine Variant Game Engine
//
// This file is distributed under the MIT License. See LICENSE.txt for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Base class to be used for classes in the engine that wish to be
/// singletons.
///
/// Based off Ogre's singleton class:
/// http://www.ogre3d.org/docs/api/1.8/_ogre_singleton_8h_source.html
///
//===----------------------------------------------------------------------===//
#pragma once

#include <cassert>

namespace sv {
template <typename T> class Singleton {

  protected:
    static T *singleton;

  public:
    Singleton(void) {
        assert(singleton == nullptr);
        singleton = static_cast<T *>(this);
    }

    ~Singleton(void) {
        assert(singleton != nullptr);
        singleton = nullptr;
    }

    static T &getSingleton(void) {
        assert(singleton != nullptr);
        return *singleton;
    }

    static T *getSingletonPtr(void) { return singleton; }

  private:
    // Prevent copying
    Singleton(const Singleton<T> &);

    Singleton &operator=(const Singleton<T> &);
};
}
