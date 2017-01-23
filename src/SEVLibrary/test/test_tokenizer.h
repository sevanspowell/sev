#include <cstring>

extern "C" {
#include <sev/tokenizer.h>
}

// Test ' quote inside argument is valid
TEST(Tokenizer, InsideSingleQuote) {
    const char *str = "echo Wo'r'ld";
    size_t strLen   = strlen(str) + 1;
    char *inputLine = (char *)malloc(sizeof(char) * strLen);
    memset(inputLine, '\0', strLen);
    snprintf(inputLine, strLen, "%s", str);

    EXPECT_EQ(0, svTokenizerValidateInput(inputLine, NULL));
    EXPECT_EQ(2, svTokenizerGetNumTokens(inputLine, NULL));
    char *token[2];
    EXPECT_EQ(0, svTokenizerTokenize(inputLine, token, NULL));
    EXPECT_TRUE(strcmp("echo", token[0]) == 0);
    EXPECT_TRUE(strcmp("Wo'r'ld", token[1]) == 0);

    free(inputLine);
}

// Test " quote inside argument is valid
TEST(Tokenizer, InsideDoubleQuote) {
    const char *str = "echo Wo\"r\"ld";
    size_t strLen   = strlen(str) + 1;
    char *inputLine = (char *)malloc(sizeof(char) * strLen);
    memset(inputLine, '\0', strLen);
    snprintf(inputLine, strLen, "%s", str);

    EXPECT_EQ(0, svTokenizerValidateInput(inputLine, NULL));
    EXPECT_EQ(2, svTokenizerGetNumTokens(inputLine, NULL));
    char *token[2];
    EXPECT_EQ(0, svTokenizerTokenize(inputLine, token, NULL));
    EXPECT_TRUE(strcmp("echo", token[0]) == 0);
    EXPECT_TRUE(strcmp("Wo\"r\"ld", token[1]) == 0);

    free(inputLine);
}

// Test that using a single ' character within a string doesn't cause an error.
TEST(Tokenizer, SingleQuoteWithinString) {
    const char *str = "echo \"Hell'o\"";
    size_t strLen   = strlen(str) + 1;
    char *inputLine = (char *)malloc(sizeof(char) * strLen);
    memset(inputLine, '\0', strLen);
    snprintf(inputLine, strLen, "%s", str);

    EXPECT_EQ(0, svTokenizerValidateInput(inputLine, NULL));
    EXPECT_EQ(2, svTokenizerGetNumTokens(inputLine, NULL));
    char *token[2];
    EXPECT_EQ(0, svTokenizerTokenize(inputLine, token, NULL));
    EXPECT_TRUE(strcmp("echo", token[0]) == 0);
    EXPECT_TRUE(strcmp("\"Hell'o\"", token[1]) == 0);

    free(inputLine);
}

// Test that using a single " character within a string doesn't cause an error.
TEST(Tokenizer, DoubleQuoteWithinString) {
    const char *str = "echo 'Hell\"o'";
    size_t strLen   = strlen(str) + 1;
    char *inputLine = (char *)malloc(sizeof(char) * strLen);
    memset(inputLine, '\0', strLen);
    snprintf(inputLine, strLen, "%s", str);

    EXPECT_EQ(0, svTokenizerValidateInput(inputLine, NULL));
    EXPECT_EQ(2, svTokenizerGetNumTokens(inputLine, NULL));
    char *token[2];
    EXPECT_EQ(0, svTokenizerTokenize(inputLine, token, NULL));
    EXPECT_TRUE(strcmp("echo", token[0]) == 0);
    EXPECT_TRUE(strcmp("'Hell\"o'", token[1]) == 0);

    free(inputLine);
}

// Test that the tokenizer complains if all opened strings are not closed.
TEST(Tokenizer, StringNotClosedSingleQuote) {
    const char *str = "echo 'Hell'o'";
    size_t strLen   = strlen(str) + 1;
    char *inputLine = (char *)malloc(sizeof(char) * strLen);
    memset(inputLine, '\0', strLen);
    snprintf(inputLine, strLen, "%s", str);

    svTokenizerError error;
    EXPECT_TRUE(svTokenizerValidateInput(inputLine, &error) != 0);
    EXPECT_EQ(-1, error.errorCode);
    EXPECT_EQ(12, error.characterPos);
    EXPECT_TRUE(svTokenizerGetNumTokens(inputLine, NULL) < 0);
    char *token[2];
    EXPECT_TRUE(svTokenizerTokenize(inputLine, token, NULL) != 0);

    free(inputLine);
}

// Test that the tokenizer complains if all opened strings are not closed.
TEST(Tokenizer, StringNotClosedDoubleQuote) {
    const char *str = "echo \"Hell\"o\"";
    size_t strLen   = strlen(str) + 1;
    char *inputLine = (char *)malloc(sizeof(char) * strLen);
    memset(inputLine, '\0', strLen);
    snprintf(inputLine, strLen, "%s", str);

    svTokenizerError error;
    EXPECT_TRUE(svTokenizerValidateInput(inputLine, &error) != 0);
    EXPECT_EQ(-1, error.errorCode);
    EXPECT_EQ(12, error.characterPos);
    EXPECT_TRUE(svTokenizerGetNumTokens(inputLine, NULL) < 0);
    char *token[2];
    EXPECT_TRUE(svTokenizerTokenize(inputLine, token, NULL) != 0);

    free(inputLine);
}

// Test that the tokenizer complains if a single quote is used not inside a
// string
TEST(Tokenizer, StringsNotClosedSingleQuote) {
    const char *str = "echo Hell'o";
    size_t strLen   = strlen(str) + 1;
    char *inputLine = (char *)malloc(sizeof(char) * strLen);
    memset(inputLine, '\0', strLen);
    snprintf(inputLine, strLen, "%s", str);

    svTokenizerError error;
    EXPECT_TRUE(svTokenizerValidateInput(inputLine, &error) != 0);
    EXPECT_EQ(-1, error.errorCode);
    EXPECT_EQ(9, error.characterPos);
    EXPECT_TRUE(svTokenizerGetNumTokens(inputLine, NULL) < 0);
    char *token[2];
    EXPECT_TRUE(svTokenizerTokenize(inputLine, token, NULL) != 0);

    free(inputLine);
}

// Test that the tokenizer complains if a double quote is used not inside a
// string
TEST(Tokenizer, StringsNotClosedDoubleQuote) {
    const char *str = "echo Hell\"o";
    size_t strLen   = strlen(str) + 1;
    char *inputLine = (char *)malloc(sizeof(char) * strLen);
    memset(inputLine, '\0', strLen);
    snprintf(inputLine, strLen, "%s", str);

    svTokenizerError error;
    EXPECT_TRUE(svTokenizerValidateInput(inputLine, &error) != 0);
    EXPECT_EQ(-1, error.errorCode);
    EXPECT_EQ(9, error.characterPos);
    EXPECT_TRUE(svTokenizerGetNumTokens(inputLine, NULL) < 0);
    char *token[2];
    EXPECT_TRUE(svTokenizerTokenize(inputLine, token, NULL) != 0);

    free(inputLine);
}

// Test that the tokenizer complains if all opened strings are not closed.
TEST(Tokenizer, StringsNotClosedSingleQuoteInsideEscapedString) {
    const char *str = "echo \\\"Hell'o\\\"";
    size_t strLen   = strlen(str) + 1;
    char *inputLine = (char *)malloc(sizeof(char) * strLen);
    memset(inputLine, '\0', strLen);
    snprintf(inputLine, strLen, "%s", str);

    svTokenizerError error;
    EXPECT_TRUE(svTokenizerValidateInput(inputLine, &error) != 0);
    EXPECT_EQ(-1, error.errorCode);
    EXPECT_EQ(11, error.characterPos);
    EXPECT_TRUE(svTokenizerGetNumTokens(inputLine, NULL) < 0);
    char *token[2];
    EXPECT_TRUE(svTokenizerTokenize(inputLine, token, NULL) != 0);

    free(inputLine);
}

// Test that the tokenizer complains if a string is opened at beginning of a
// token and not closed (single quote)
TEST(Tokenizer, StringsNotClosedBeginningTokenSingleQuote) {
    const char *str = "echo 'Hello";
    size_t strLen   = strlen(str) + 1;
    char *inputLine = (char *)malloc(sizeof(char) * strLen);
    memset(inputLine, '\0', strLen);
    snprintf(inputLine, strLen, "%s", str);

    svTokenizerError error;
    EXPECT_TRUE(svTokenizerValidateInput(inputLine, &error) != 0);
    EXPECT_EQ(-1, error.errorCode);
    EXPECT_EQ(5, error.characterPos);
    EXPECT_TRUE(svTokenizerGetNumTokens(inputLine, NULL) < 0);
    char *token[2];
    EXPECT_TRUE(svTokenizerTokenize(inputLine, token, NULL) != 0);

    free(inputLine);
}

// Test that the tokenizer complains if a string is opened at beginning of a
// token and not closed (double quote)
TEST(Tokenizer, StringsNotClosedBeginningTokenDoubleQuote) {
    const char *str = "echo \"Hello";
    size_t strLen   = strlen(str) + 1;
    char *inputLine = (char *)malloc(sizeof(char) * strLen);
    memset(inputLine, '\0', strLen);
    snprintf(inputLine, strLen, "%s", str);

    svTokenizerError error;
    EXPECT_TRUE(svTokenizerValidateInput(inputLine, &error) != 0);
    EXPECT_EQ(-1, error.errorCode);
    EXPECT_EQ(5, error.characterPos);
    EXPECT_TRUE(svTokenizerGetNumTokens(inputLine, NULL) < 0);
    char *token[2];
    EXPECT_TRUE(svTokenizerTokenize(inputLine, token, NULL) != 0);

    free(inputLine);
}

// Test that the tokenizer complains if a string is opened at end of a
// token and not closed (single quote)
TEST(Tokenizer, StringsNotClosedEndTokenSingleQuote) {
    const char *str = "echo Hello'";
    size_t strLen   = strlen(str) + 1;
    char *inputLine = (char *)malloc(sizeof(char) * strLen);
    memset(inputLine, '\0', strLen);
    snprintf(inputLine, strLen, "%s", str);

    svTokenizerError error;
    EXPECT_TRUE(svTokenizerValidateInput(inputLine, &error) != 0);
    EXPECT_EQ(-1, error.errorCode);
    EXPECT_EQ(10, error.characterPos);
    EXPECT_TRUE(svTokenizerGetNumTokens(inputLine, NULL) < 0);
    char *token[2];
    EXPECT_TRUE(svTokenizerTokenize(inputLine, token, NULL) != 0);

    free(inputLine);
}

// Test that the tokenizer complains if a string is opened at end of a
// token and not closed (double quote)
TEST(Tokenizer, StringsNotClosedEndTokenDoubleQuote) {
    const char *str = "echo Hello\"";
    size_t strLen   = strlen(str) + 1;
    char *inputLine = (char *)malloc(sizeof(char) * strLen);
    memset(inputLine, '\0', strLen);
    snprintf(inputLine, strLen, "%s", str);

    svTokenizerError error;
    EXPECT_TRUE(svTokenizerValidateInput(inputLine, &error) != 0);
    EXPECT_EQ(-1, error.errorCode);
    EXPECT_EQ(10, error.characterPos);
    EXPECT_TRUE(svTokenizerGetNumTokens(inputLine, NULL) < 0);
    char *token[2];
    EXPECT_TRUE(svTokenizerTokenize(inputLine, token, NULL) != 0);

    free(inputLine);
}

// Test that the tokenizer works properly with escaped spaces
TEST(Tokenizer, EscapedSpace) {
    const char *str = "echo Hello\\ World";
    size_t strLen   = strlen(str) + 1;
    char *inputLine = (char *)malloc(sizeof(char) * strLen);
    memset(inputLine, '\0', strLen);
    snprintf(inputLine, strLen, "%s", str);

    EXPECT_EQ(0, svTokenizerValidateInput(inputLine, NULL));
    EXPECT_EQ(2, svTokenizerGetNumTokens(inputLine, NULL));
    char *token[2];
    EXPECT_EQ(0, svTokenizerTokenize(inputLine, token, NULL));
    EXPECT_TRUE(strcmp("echo", token[0]) == 0);
    EXPECT_TRUE(strcmp("Hello\\ World", token[1]) == 0);

    free(inputLine);
}

// Test that a trailing escape character does not cause an error
TEST(Tokenizer, TrailingEscapeCharacter) {
    const char *str = "echo Hello\\";
    size_t strLen   = strlen(str) + 1;
    char *inputLine = (char *)malloc(sizeof(char) * strLen);
    memset(inputLine, '\0', strLen);
    snprintf(inputLine, strLen, "%s", str);

    EXPECT_EQ(0, svTokenizerValidateInput(inputLine, NULL));
    EXPECT_EQ(2, svTokenizerGetNumTokens(inputLine, NULL));
    char *token[2];
    EXPECT_EQ(0, svTokenizerTokenize(inputLine, token, NULL));
    EXPECT_TRUE(strcmp("echo", token[0]) == 0);
    EXPECT_TRUE(strcmp("Hello\\", token[1]) == 0);

    free(inputLine);
}

// Test inner string (single quotes)
TEST(Tokenizer, InnerStringSingleQuotes) {
    const char *str = "echo 'Hello'o'p'";
    size_t strLen   = strlen(str) + 1;
    char *inputLine = (char *)malloc(sizeof(char) * strLen);
    memset(inputLine, '\0', strLen);
    snprintf(inputLine, strLen, "%s", str);

    EXPECT_EQ(0, svTokenizerValidateInput(inputLine, NULL));
    EXPECT_EQ(2, svTokenizerGetNumTokens(inputLine, NULL));
    char *token[2];
    EXPECT_EQ(0, svTokenizerTokenize(inputLine, token, NULL));
    EXPECT_TRUE(strcmp("echo", token[0]) == 0);
    EXPECT_TRUE(strcmp("'Hello'o'p'", token[1]) == 0);

    free(inputLine);
}

// Test inner string (double quotes)
TEST(Tokenizer, InnerStringDoubleQuote) {
    const char *str = "echo \"Hello\"o\"p\"";
    size_t strLen   = strlen(str) + 1;
    char *inputLine = (char *)malloc(sizeof(char) * strLen);
    memset(inputLine, '\0', strLen);
    snprintf(inputLine, strLen, "%s", str);

    EXPECT_EQ(0, svTokenizerValidateInput(inputLine, NULL));
    EXPECT_EQ(2, svTokenizerGetNumTokens(inputLine, NULL));
    char *token[2];
    EXPECT_EQ(0, svTokenizerTokenize(inputLine, token, NULL));
    EXPECT_TRUE(strcmp("echo", token[0]) == 0);
    EXPECT_TRUE(strcmp("\"Hello\"o\"p\"", token[1]) == 0);

    free(inputLine);
}

// Test inner string (single quotes) inside double quotes string
TEST(Tokenizer, InnerStringSingleQuotesInsideDoubleQuotes) {
    const char *str = "echo \"Hello'o'p\"";
    size_t strLen   = strlen(str) + 1;
    char *inputLine = (char *)malloc(sizeof(char) * strLen);
    memset(inputLine, '\0', strLen);
    snprintf(inputLine, strLen, "%s", str);

    EXPECT_EQ(0, svTokenizerValidateInput(inputLine, NULL));
    EXPECT_EQ(2, svTokenizerGetNumTokens(inputLine, NULL));
    char *token[2];
    EXPECT_EQ(0, svTokenizerTokenize(inputLine, token, NULL));
    EXPECT_TRUE(strcmp("echo", token[0]) == 0);
    EXPECT_TRUE(strcmp("\"Hello'o'p\"", token[1]) == 0);

    free(inputLine);
}

// Test inner string (double quotes) inside single quotes string
TEST(Tokenizer, InnerStringDoubleQuoteInsideSingleQuotes) {
    const char *str = "echo 'Hello\"o\"p'";
    size_t strLen   = strlen(str) + 1;
    char *inputLine = (char *)malloc(sizeof(char) * strLen);
    memset(inputLine, '\0', strLen);
    snprintf(inputLine, strLen, "%s", str);

    EXPECT_EQ(0, svTokenizerValidateInput(inputLine, NULL));
    EXPECT_EQ(2, svTokenizerGetNumTokens(inputLine, NULL));
    char *token[2];
    EXPECT_EQ(0, svTokenizerTokenize(inputLine, token, NULL));
    EXPECT_TRUE(strcmp("echo", token[0]) == 0);
    EXPECT_TRUE(strcmp("'Hello\"o\"p'", token[1]) == 0);

    free(inputLine);
}

// Test empty string using single quotes
TEST(Tokenizer, EmptyStringSingleQuotes) {
    const char *str = "echo ''";
    size_t strLen   = strlen(str) + 1;
    char *inputLine = (char *)malloc(sizeof(char) * strLen);
    memset(inputLine, '\0', strLen);
    snprintf(inputLine, strLen, "%s", str);

    EXPECT_EQ(0, svTokenizerValidateInput(inputLine, NULL));
    EXPECT_EQ(2, svTokenizerGetNumTokens(inputLine, NULL));
    char *token[2];
    EXPECT_EQ(0, svTokenizerTokenize(inputLine, token, NULL));
    EXPECT_TRUE(strcmp("echo", token[0]) == 0);
    EXPECT_TRUE(strcmp("", token[1]) == 0);

    free(inputLine);
}

// Test empty string using double quotes
TEST(Tokenizer, EmptyStringDoubleQuotes) {
    const char *str = "echo \"\"";
    size_t strLen   = strlen(str) + 1;
    char *inputLine = (char *)malloc(sizeof(char) * strLen);
    memset(inputLine, '\0', strLen);
    snprintf(inputLine, strLen, "%s", str);

    EXPECT_EQ(0, svTokenizerValidateInput(inputLine, NULL));
    EXPECT_EQ(2, svTokenizerGetNumTokens(inputLine, NULL));
    char *token[2];
    EXPECT_EQ(0, svTokenizerTokenize(inputLine, token, NULL));
    EXPECT_TRUE(strcmp("echo", token[0]) == 0);
    EXPECT_TRUE(strcmp("", token[1]) == 0);

    free(inputLine);
}

// Test inner quote present at end of string using single quotes
TEST(Tokenizer, InnerQuoteAtEndOfStringSingleQuotes) {
    const char *str = "echo 'Hello'o''";
    size_t strLen   = strlen(str) + 1;
    char *inputLine = (char *)malloc(sizeof(char) * strLen);
    memset(inputLine, '\0', strLen);
    snprintf(inputLine, strLen, "%s", str);

    EXPECT_EQ(0, svTokenizerValidateInput(inputLine, NULL));
    EXPECT_EQ(2, svTokenizerGetNumTokens(inputLine, NULL));
    char *token[2];
    EXPECT_EQ(0, svTokenizerTokenize(inputLine, token, NULL));
    EXPECT_TRUE(strcmp("echo", token[0]) == 0);
    EXPECT_TRUE(strcmp("'Hello'o", token[1]) == 0);

    free(inputLine);
}

// Test inner quote present at end of string using double quotes
TEST(Tokenizer, InnerQuoteAtEndOfStringDoubleQuotes) {
    const char *str = "echo \"Hello\"o\"\"";
    size_t strLen   = strlen(str) + 1;
    char *inputLine = (char *)malloc(sizeof(char) * strLen);
    memset(inputLine, '\0', strLen);
    snprintf(inputLine, strLen, "%s", str);

    EXPECT_EQ(0, svTokenizerValidateInput(inputLine, NULL));
    EXPECT_EQ(2, svTokenizerGetNumTokens(inputLine, NULL));
    char *token[2];
    EXPECT_EQ(0, svTokenizerTokenize(inputLine, token, NULL));
    EXPECT_TRUE(strcmp("echo", token[0]) == 0);
    EXPECT_TRUE(strcmp("\"Hello\"o", token[1]) == 0);

    free(inputLine);
}

// Test inner quote present in middle of string using single quotes
TEST(Tokenizer, InnerQuoteMiddleOfStringSingleQuotes) {
    const char *str = "echo 'Hello'o''world";
    size_t strLen   = strlen(str) + 1;
    char *inputLine = (char *)malloc(sizeof(char) * strLen);
    memset(inputLine, '\0', strLen);
    snprintf(inputLine, strLen, "%s", str);

    EXPECT_EQ(0, svTokenizerValidateInput(inputLine, NULL));
    EXPECT_EQ(2, svTokenizerGetNumTokens(inputLine, NULL));
    char *token[2];
    EXPECT_EQ(0, svTokenizerTokenize(inputLine, token, NULL));
    EXPECT_TRUE(strcmp("echo", token[0]) == 0);
    EXPECT_TRUE(strcmp("'Hello'oworld", token[1]) == 0);

    free(inputLine);
}

// Test inner quote present in middle of string using double quotes
TEST(Tokenizer, InnerQuoteMiddleOfStringDoubleQuotes) {
    const char *str = "echo \"Hello\"o\"\"world";
    size_t strLen   = strlen(str) + 1;
    char *inputLine = (char *)malloc(sizeof(char) * strLen);
    memset(inputLine, '\0', strLen);
    snprintf(inputLine, strLen, "%s", str);

    EXPECT_EQ(0, svTokenizerValidateInput(inputLine, NULL));
    EXPECT_EQ(2, svTokenizerGetNumTokens(inputLine, NULL));
    char *token[2];
    EXPECT_EQ(0, svTokenizerTokenize(inputLine, token, NULL));
    EXPECT_TRUE(strcmp("echo", token[0]) == 0);
    EXPECT_TRUE(strcmp("\"Hello\"oworld", token[1]) == 0);

    free(inputLine);
}

// Test that a simple valid example works
TEST(Tokenizer, Valid) {
    const char *str = "echo \"Hello World\"";
    size_t strLen   = strlen(str) + 1;
    char *inputLine = (char *)malloc(sizeof(char) * strLen);
    memset(inputLine, '\0', strLen);
    snprintf(inputLine, strLen, "%s", str);

    EXPECT_EQ(0, svTokenizerValidateInput(inputLine, NULL));
    EXPECT_EQ(2, svTokenizerGetNumTokens(inputLine, NULL));
    char *token[2];
    EXPECT_EQ(0, svTokenizerTokenize(inputLine, token, NULL));
    EXPECT_TRUE(strcmp("echo", token[0]) == 0);
    EXPECT_TRUE(strcmp("\"Hello World\"", token[1]) == 0);

    free(inputLine);
}
