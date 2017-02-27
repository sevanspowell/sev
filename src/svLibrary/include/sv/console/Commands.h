/* Command seperator - must be a single character */
#define SV_SEP_SEQ ";"

typedef struct svCommand {
    int first; /* Index of first token of this command in the token array */
    int last;  /* Index of last token of this command in the token array */
    const char *sep; /* The command seperator that follows the command */
    char **argv;     /* The array of characters that forms the command */
} svCommand;

typedef struct svCommandError {
    int errorCode; /* Error code:
                    *     -1, if two successive commands are seperated by more
                    *     than one command separator.
                    *     -2, if the first token is a command separator.
                    */
    int tokenPos;  /* Position of token causing error */
} svCommandError;

/**
 * \returns 1 if the given character is a command separator, 0 if it is not.
 */
int svCommandsIsSep(char token);

/**
 * Analyze the given token array and determine whether or not the given token
 * array is valid.
 *
 * This does not have to be called before 'svCommandsGetNum' or
 * 'svCommandsSeparate', but if done so and this function returns success, it
 * guarantees those functions will succeed too.
 *
 * \param token        array of tokens with last element being NULL.
 * \param error [out]  pointer to error structure which is filled in case of
 * error in token array.
 *
 * \returns 0 if no error, other if error.
 */
int svCommandsValidateTokenArray(char *token[], svCommandError *error);

/**
 * Analyze the given token array and determine the number of 'svCommand'
 * structures are required to hold all the commands. This number of 'svCommand'
 * structures (plus one for 'NULL' command) should be allocated before being
 * handed to 'svCommandsSeparate'.
 *
 * \note Number does not include the last element of the command array which
 * must be a 'NULL' command.
 *
 * \param token        array of tokens with last element being NULL.
 * \param error [out]  pointer to error structure which is filled in case of
 * error in token array.
 *
 * \returns number of 'svCommand' structures (not including NULL command) that
 * should be allocated to hold all commands in the given token array or < 0 in
 * case of error.
 */
int svCommandsGetNum(char *token[], svCommandError *error);

/**
 * Separate the list of tokens in the \p token array into a sequence of
 * commands, which are stored in the \p command array.
 *
 * \pre The \p command array must have enough elements to store all commands,
 * see 'svCommandsGetNum'.
 * \pre the \p command array's last element must have all members set to 0.
 *
 * \param token          array of tokens with last element being NULL.
 * \param command [out]  array of commands out.
 * \param error   [out]  pointer to error structure which is filled in case of
 * error in token array.
 *
 * \returns 0 if no error, other if error.
 */
int svCommandsSeparate(char *token[], svCommand command[],
                       svCommandError *error);

/**
 * Frees any of the contents of an array of commands that was dynamically
 * allocated.
 *
 * Should be called after 'svCommandsSeparate', when finished with commands.
 *
 * \pre Last element of command array must be a 'NULL' command (all members set
 * to 0).
 *
 * \param command [in/out] array of commands to free contents of.
 */
void svCommandsFreeContents(svCommand command[]);

/**
 * Make the given \p command a 'NULL' command, used for indicating the end of a
 * command array.
 *
 * \pre Given command should either have not been used yet or just freed using
 * 'svCommandsFreeContents'.
 *
 * \param command [out] command to make a 'NULL' command.
 */
void svCommandsMakeNullCommand(svCommand *command);
