#include <sv/Common.h>

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
}
