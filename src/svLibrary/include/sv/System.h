//===-- sv/System.h - Determine platform/compiler ====-----------*- C++ -*-===//
//
//                 The Special Engine Variant Game Engine
//
// This file is distributed under the MIT License. See LICENSE.txt for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Collection of #defines used to determine platform/compiler we are
/// compiling for.
///
//===----------------------------------------------------------------------===//
/*
 * Adapted from Daniele Bartolini's platform
 * https://github.com/taylor001/crown/blob/master/src/core/platform.h:
 *
 * Modified by Samuel Evans-Powell.
 */
#pragma once

#define SV_COMPILER_CLANG 0
#define SV_COMPILER_GCC 0
#define SV_COMPILER_MSVC 0

#define SV_PLATFORM_LINUX 0
#define SV_PLATFORM_OSX 0
#define SV_PLATFORM_WINDOWS 0

// https://sourceforge.net/p/predef/wiki/Compilers/
#if defined(_MSC_VER)
#undef SV_COMPILER_MSVC
#define SV_COMPILER_MSVC 1
#elif defined(__clang__)
// clang defines __GNUC__
#undef SV_COMPILER_CLANG
#define SV_COMPILER_CLANG 1
#undef SV_COMPILER_GCC
#define SV_COMPILER_GCC 1
#elif defined(__GNUC__)
#undef SV_COMPILER_GCC
#define SV_COMPILER_GCC 1
#else
#error "SV_COMPILER_* is not defined!"
#endif

// https://sourceforge.net/p/predef/wiki/OperatingSystems/
#if defined(_WIN32) || defined(_WIN64)
#undef SV_PLATFORM_WINDOWS
#define SV_PLATFORM_WINDOWS 1
#elif defined(__linux__)
#undef SV_PLATFORM_LINUX
#define SV_PLATFORM_LINUX 1
#elif defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__)
#undef SV_PLATFORM_OSX
#define SV_PLATFORM_OSX 1
#else
#error "SV_PLATFORM_* is not defined!"
#endif

#define SV_PLATFORM_POSIX (SV_PLATFORM_LINUX || SV_PLATFORM_OSX)

#if SV_PLATFORM_LINUX
#define SV_PLATFORM_NAME "linux"
#elif SV_PLATFORM_OSX
#define SV_PLATFORM_NAME "osx"
#elif SV_PLATFORM_WINDOWS
#define SV_PLATFORM_NAME "windows"
#endif

// Force inlining
#if SV_COMPILER_MSVC
#define SV_FORCE_INLINE __forceinline
#elif SV_COMPILER_GCC
#define SV_FORCE_INLINE __attribute__((always_inline))
#endif
