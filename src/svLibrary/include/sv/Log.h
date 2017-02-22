//===-- sv/Log.h - Logging --------------------------------------*- C++ -*-===//
//
//                 The Special Engine Variant Game Engine
//
// This file is distributed under the MIT License. See LICENSE.txt for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Collection of classes to aid in logging
///
/// Based off GmbH's "Logging in a Multithreaded Environment" article:
/// http://blog.instance-factory.com/?p=181
///
//===----------------------------------------------------------------------===//
#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace sv {

namespace LogArea {
enum Enum : uint32_t {
    COMMON   = 1 << 0,
    CONSOLE  = 1 << 1,
    PLATFORM = 1 << 2,
    ALL      = ~(uint32_t)0
};
}

namespace LogLevel {
enum Enum : uint32_t {
    ERROR   = 1 << 0,
    EVENT   = 1 << 1,
    DEBUG   = 1 << 2,
    WARNING = 1 << 3,
    ALL     = ~(uint32_t)0
};
}

/// A log message
class LogMessage {
  public:
    LogMessage(LogArea::Enum logArea, LogLevel::Enum logLevel,
               const std::string &logText)
        : area(logArea), level(logLevel), text(logText) {}

    const LogArea::Enum area;
    const LogLevel::Enum level;
    const std::string text;
};

/// Abstract base class that observes some log areas for messages.
class LogObserver {
  public:
    // \param   observeAreas   Bitwise OR'd set of LogMessage::Area flags.
    // \param   observeLevels  Bitwise OR'd set of LogMessage::Level flags.
    LogObserver(uint32_t observeAreas, uint32_t observeLevels)
        : areas(observeAreas), levels(observeLevels) {}

    virtual ~LogObserver() {}

    /// Write a log message
    virtual void write(const LogMessage &message) = 0;

    /// \returns Bitwise OR'd set of flags indicating log areas observed, see
    /// LogMessage::Area.
    uint32_t getAreas();

    /// \returns Bitwise OR'd set of flags indicating log levels observed, see
    /// LogMessage::Level.
    uint32_t getLevels();

  private:
    const uint32_t areas;
    const uint32_t levels;
};

/// Log messages here, distributes log messages to registered observers.
class LogDistributor {
  public:
    /// Register the given observer to receive log messages when 'logMessage' is
    /// called. Each observer specifies which log areas and levels it wishes to
    /// observe.
    void registerObserver(const std::shared_ptr<LogObserver> &observer);

    /// Remove the given observer from the log distributor
    void removeObserver(const std::shared_ptr<LogObserver> &observer);

    /// Log the given message. Calling the appropriate observers.
    void logMessage(const LogMessage &message);

  private:
    typedef std::vector<std::shared_ptr<LogObserver> > ObserverList;
    ObserverList observerList;
};
}
