#pragma once

typedef struct svTokenizerError {
    /* Error code:
     *     -1,  if string is not properly closed, e.g. ("Hello world) instead of
     *          ("Hello world")
     */
    int errorCode;
    /* Position of character causing error relative to starting character of
     * input */
    int characterPos;
} svTokenizerError;

/**
 * Analyze the given input line and determine whether or not it is valid input
 * for the tokenizer.
 *
 * This does not have to be called before calling 'svTokenizerGetNumTokens' or
 * 'svTokenizerTokenize' but if done so and this function returns success, it
 * guarentees those functions will succeed too.
 *
 * \param       inputLine    string of characters to validate for tokenizer.
 * \param [out] error        pointer to error structure which is filled in case
 * of error.
 *
 * \return 0 if no error, other if error.
 */
int svTokenizerValidateInput(const char *inputLine, svTokenizerError *error);

/**
 * Analyze the given input line and determine the number of tokens required to
 * hold the input line.
 *
 * \note Does not include NULL token required for passing to svCommandsSeparate.
 *
 * \param       inputLine    string of characters to validate for tokenizer.
 * \param [out] error        pointer to error structure which is filled in case
 * of error.
 *
 * \return Number of tokens required to hold input line if no error (>= 0) or <
 * 0 on error.
 */
int svTokenizerGetNumTokens(const char *inputLine, svTokenizerError *error);

/**
 * Given a string of characters provided in \p inputLine, splits the
 * characters
 * into a series of tokens and puts them in the output array \p token.
 *
 * If successful, returns the number of tokens parsed. If an error occurs,
 * one
 * of the following error codes will be returned:
 *     -1 - if inputLine contains > MAX_NUM_TOKENS
 *     -2 - if string is not properly closed, e.g. ("Hello world) instead of
 *          ("Hello world")
 *
 * \pre   \p inputLine is a NULL-terminated string.
 * \pre   \p token is an array large enough to hold at least MAX_NUM_TOKENS
 *
 * \param       inputLine    string of characters to tokenise.
 * \param [out] token        tokens out.
 * \param [out] error        pointer to error structure which is filled in case
 * of error.
 *
 * \return 0 if no error, other if error.
*/
int svTokenizerTokenize(char *inputLine, char *token[],
                        svTokenizerError *error);
