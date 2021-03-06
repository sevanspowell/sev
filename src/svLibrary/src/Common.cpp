#include <sv/Common.h>
#include <sv/System.h>

#if SV_PLATFORM_POSIX
#include <time.h>
#elif SV_PLATFORM_WINDOWS
#endif

namespace sv {
char *stripSurroundingQuotes(char *str) {
    char *result = str;
    char *end    = str;

    if (*str != '\0') {
        // Move 'end' to end of string
        while (*end != '\0') {
            ++end;
        }
        --end;

        if ((*str == '\'' && *end == '\'') || (*str == '"' && *end == '"')) {
            *end   = '\0';
            result = str + 1;
        }
    }

    return result;
}

// The following function was found on
// http://xoomer.virgilio.it/acantato/dev/wildcard/wildmatch.html, where it was
// attributed to the C/C++ Users Journal, written by Mike Cornelison.
bool wildcardMatch(const char *pat, const char *str) {
    int i, star;

new_segment:

    star = 0;
    if (*pat == '*') {
        star = 1;
        do {
            pat++;
        } while (*pat == '*'); /* enddo */
    }                          /* endif */

test_match:

    for (i = 0; pat[i] && (pat[i] != '*'); i++) {
        // if (mapCaseTable[str[i]] != mapCaseTable[pat[i]]) {
        if (str[i] != pat[i]) {
            if (!str[i])
                return false;
            if ((pat[i] == '?') && (str[i] != '.'))
                continue;
            if (!star)
                return false;
            str++;
            goto test_match;
        }
    }
    if (pat[i] == '*') {
        str += i;
        pat += i;
        goto new_segment;
    }
    if (!str[i])
        return true;
    if (i && pat[i - 1] == '*')
        return true;
    if (!star)
        return false;
    str++;
    goto test_match;
}

void sleep(float seconds) {
    if (seconds >= 0) {
#if SV_PLATFORM_POSIX
        struct timespec t;
        t.tv_sec  = (time_t)seconds;
        t.tv_nsec = 0;
        nanosleep(&t, NULL);
#elif SV_PLATFORM_WINDOWS
        Sleep((int)(seconds * 1000.0f));
#endif
    }
}
}
