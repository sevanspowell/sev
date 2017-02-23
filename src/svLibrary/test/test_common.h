#include <cstdio>
#include <cstring>

#include <sv/Common.h>

TEST(Common, StripSurroundingSingleQuotes) {
    char *str = (char *)malloc(sizeof(char) * 6);
    strncpy(str, "'Hoo'", 6);
    char *stripped = sv::stripSurroundingQuotes(str);

    EXPECT_TRUE(strcmp(stripped, "Hoo") == 0);

    free(str);
}

TEST(Common, StripSurroundingDoubleQuotes) {
    char *str = (char *)malloc(sizeof(char) * 6);
    strncpy(str, "\"Hoo\"", 6);
    char *stripped = sv::stripSurroundingQuotes(str);

    EXPECT_TRUE(strcmp(stripped, "Hoo") == 0);

    free(str);
}

// Function shouldn't do anything if either start or end characters aren't
// quotes
TEST(Common, StripSurroundingQuotesNoChange1) {
    char *str = (char *)malloc(sizeof(char) * 5);
    strncpy(str, "Hoo\"", 5);
    char *stripped = sv::stripSurroundingQuotes(str);

    EXPECT_TRUE(strcmp(stripped, "Hoo\"") == 0);

    free(str);
}

// Function shouldn't do anything if either start or end characters aren't
// quotes
TEST(Common, StripSurroundingQuotesNoChange2) {
    char *str = (char *)malloc(sizeof(char) * 4);
    strncpy(str, "Hoo", 4);
    char *stripped = sv::stripSurroundingQuotes(str);

    EXPECT_TRUE(strcmp(stripped, "Hoo") == 0);

    free(str);
}

// Function shouldn't do anything if either start or end characters aren't
// quotes
TEST(Common, StripSurroundingQuotesNoChange3) {
    char *str = (char *)malloc(sizeof(char) * 5);
    strncpy(str, "'Hoo", 5);
    char *stripped = sv::stripSurroundingQuotes(str);

    EXPECT_TRUE(strcmp(stripped, "'Hoo") == 0);

    free(str);
}
