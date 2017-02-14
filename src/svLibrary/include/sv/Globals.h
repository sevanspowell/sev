#pragma once

#include <memory>

#include <sv/Log.h>

namespace sv_globals {
static std::unique_ptr<sv::LogDistributor> logger(nullptr);

inline void log(sv::LogArea::Enum area, sv::LogLevel::Enum level,
                const std::string &message) {
    // Don't lazily initialize logger, because if it hasn't been initialized
    // yet, it also doesn't have any registered observers, so writing to the log
    // distributor will do nothing even if we do initialize it.
    if (logger != nullptr) {
        logger->logMessage(sv::LogMessage(area, level, message));
    }
}
}
