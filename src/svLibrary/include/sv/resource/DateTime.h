//===-- sv/resource/DateTime.h - Date/time structure ------------*- C++ -*-===//
//
//                 The Special Engine Variant Game Engine
//
// This file is distributed under the MIT License. See LICENSE.txt for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Represents some point in time. In our case it is used to represent
/// the last modified date of files in a ResourceCollection.
///
//===----------------------------------------------------------------------===//
#pragma once

#include <cstdint>

namespace sv {
///-----------------------------------------------------------------------------
/// Used to represent some point in time.
///-----------------------------------------------------------------------------
class DateTime {
  public:
    DateTime(int seconds = 0, int minutes = 0, int hours = 0, int days = 1,
             int months = 0, int years = 0)
        : sec(seconds), min(minutes), hour(hours), day(days), month(months),
          year(years) {}

    bool operator==(const DateTime &other) const;
    bool operator<(const DateTime &other) const;
    bool operator>(const DateTime &other) const;
    bool operator<=(const DateTime &other) const;
    bool operator>=(const DateTime &other) const;

    /// Number of seconds (0 - 60)
    int8_t sec;
    /// Number of minutes (0 - 59)
    int8_t min;
    /// Number of hours (0 - 23)
    int8_t hour;
    /// Day of month (1 - 31)
    int8_t day;
    /// Month of year (0 - 11)
    int8_t month;
    /// AD year - e.g. 1994 for year 1994
    int16_t year;
};
}
