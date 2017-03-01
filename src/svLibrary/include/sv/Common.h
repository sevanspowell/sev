#pragma once

namespace sv {
///-----------------------------------------------------------------------------
/// Strip surrounding quotes from a C string if the first and last characters
/// are both either a " or a ' character.
///-----------------------------------------------------------------------------
char *stripSurroundingQuotes(char *str);

///-----------------------------------------------------------------------------
/// \returns True if the string \p str matches the wildcard pattern \pat.
///-----------------------------------------------------------------------------
bool wildcardMatch(const char *pat, const char *str);

///-----------------------------------------------------------------------------
/// Causes the calling thread to sleep for \p seconds seconds.
///-----------------------------------------------------------------------------
void sleep(float seconds);
}
