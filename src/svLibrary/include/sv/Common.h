#pragma once

namespace sv {
///-----------------------------------------------------------------------------
/// Strip surrounding quotes from a C string if the first and last characters
//// are both either a " or a ' character.
///-----------------------------------------------------------------------------
char *stripSurroundingQuotes(char *str);
}
