//===-- sv/Asserts.h - Asserts ----------------------------------*- C++ -*-===//
//
//                 The Special Engine Variant Game Engine
//
// This file is distributed under the MIT License. See LICENSE.txt for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Used to halt program execution in case of unrecoverable error.
///
//===----------------------------------------------------------------------===//
/*
 * Copyright (c) 2008, Power of Two Games LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * * Neither the name of Power of Two Games LLC nor the
 * names of its contributors may be used to endorse or promote products
 * derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY POWER OF TWO GAMES LLC ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL POWER OF TWO GAMES LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Original: http://cnicholson.net/wp-content/uploads/2009/02/pow2assert.h
 *
 * Modified by Samuel Evans-Powell.
 */
#pragma once

#include <sv/System.h>

#if SV_COMPILER_GCC
#include <signal.h>
#endif

#ifdef SV_BUILD_DEBUG
#define SV_ASSERTS_ENABLED
#endif

namespace sv {
namespace assert {
// Using "better enums"
// http://gamesfromwithin.com/stupid-c-tricks-2-better-enums
namespace FailBehaviour {
enum Enum { Halt, Continue };
}

typedef FailBehaviour::Enum (*Handler)(const char *condition, const char *msg,
                                       const char *file, int line);

Handler getHandler();
void setHandler(Handler newHandler);

FailBehaviour::Enum reportFailure(const char *condition, const char *file,
                                  int line, const char *msg, ...);
}
}

// Halt function to use
#if SV_COMPILER_MSVC
#define SV_HALT() __debugbreak()
#elif SV_COMPILER_GCC
#define SV_HALT() raise(SIGTRAP)
#endif

// Macro that should compile away to nothing
#define SV_UNUSED(x)                                                           \
    do {                                                                       \
        (void)sizeof(x);                                                       \
    } while (0)

#ifdef SV_ASSERTS_ENABLED
#define SV_ASSERT(cond)                                                        \
    do {                                                                       \
        if (!(cond)) {                                                         \
            if (sv::assert::reportFailure(#cond, __FILE__, __LINE__, 0) ==     \
                sv::assert::FailBehaviour::Enum::Halt)                         \
                SV_HALT();                                                     \
        }                                                                      \
    } while (0)

#define SV_ASSERT_MSG(cond, msg, ...)                                          \
    do {                                                                       \
        if (!(cond)) {                                                         \
            if (sv::assert::reportFailure(0, __FILE__, __LINE__, (msg),        \
                                          ##__VA_ARGS__) ==                    \
                sv::assert::FailBehaviour::Enum::Halt)                         \
                SV_HALT();                                                     \
        }                                                                      \
    } while (0)

#define SV_ASSERT_FAIL(msg, ...)                                               \
    do {                                                                       \
        if (sv::assert::reportFailure(0, __FILE__, __LINE__, (msg),            \
                                      ##__VA_ARGS__) ==                        \
            sv::assert::FailBehaviour::Enum::Halt)                             \
            SV_HALT();                                                         \
    } while (0)

#else
#define SV_ASSERT(cond)                                                        \
    do {                                                                       \
        SV_UNUSED(cond);                                                       \
    } while (0)

#define SV_ASSERT_MSG(cond, msg, ...)                                          \
    do {                                                                       \
        SV_UNUSED(cond);                                                       \
        SV_UNUSED(msg);                                                        \
    } while (0)

#define SV_ASSERT_FAIL(msg, ...)                                               \
    do {                                                                       \
        SV_UNUSED(msg);                                                        \
    } while (0)
#endif
