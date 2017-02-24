#include <sv/resource/DateTime.h>

namespace sv {
bool DateTime::operator==(const DateTime &other) const {
    return (sec == other.sec && min == other.min && hour == other.hour &&
            day == other.day && month == other.month && year == other.year);
}

bool DateTime::operator<(const DateTime &other) const {
    if (year < other.year) {
        return true;
    } else if (year > other.year) {
        return false;
    }
    // Year is equal...

    if (month < other.month) {
        return true;
    } else if (month > other.month) {
        return false;
    }
    // Month is equal...

    if (day < other.day) {
        return true;
    } else if (day > other.day) {
        return false;
    }
    // Day is equal...

    if (hour < other.hour) {
        return true;
    } else if (hour > other.hour) {
        return false;
    }
    // Hour is equal...

    if (min < other.min) {
        return true;
    } else if (min > other.min) {
        return false;
    }
    // Min is equal...

    if (sec < other.sec) {
        return true;
    } else if (sec > other.sec) {
        return false;
    }
    // All fields are equal...

    return false;
}

bool DateTime::operator>(const DateTime &other) const {
    if (year > other.year) {
        return true;
    } else if (year < other.year) {
        return false;
    }
    // Year is equal...

    if (month > other.month) {
        return true;
    } else if (month < other.month) {
        return false;
    }
    // Month is equal...

    if (day > other.day) {
        return true;
    } else if (day < other.day) {
        return false;
    }
    // Day is equal...

    if (hour > other.hour) {
        return true;
    } else if (hour < other.hour) {
        return false;
    }
    // Hour is equal...

    if (min > other.min) {
        return true;
    } else if (min < other.min) {
        return false;
    }
    // Min is equal...

    if (sec > other.sec) {
        return true;
    } else if (sec < other.sec) {
        return false;
    }
    // All fields are equal...

    return false;
}

bool DateTime::operator<=(const DateTime &other) const {
    return (*this == other || *this < other);
}

bool DateTime::operator>=(const DateTime &other) const {
    return (*this == other || *this > other);
}
}
