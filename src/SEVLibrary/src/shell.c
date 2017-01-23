#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sev/commands.h>
#include <sev/shell.h>
#include <sev/tokenizer.h>

#define SV_SHELL_OUT_BUFFER_SIZE 1024
static char svOutputBuffer[SV_SHELL_OUT_BUFFER_SIZE];
static char svErrorBuffer[SV_SHELL_OUT_BUFFER_SIZE];

typedef struct svShellCommand {
    const char *cmdName;
    int (*cmdFunc)(int, char **);
} svShellCommand;

#define SV_SHELL_MAX_COMMANDS 256
static svShellCommand svShellCommands[SV_SHELL_MAX_COMMANDS];
static int svShellCommandsNum = 0;

int svShellAddCommand(const char *commandName,
                      int (*commandFunction)(int, char **)) {
    if (svShellCommandsNum == SV_SHELL_MAX_COMMANDS - 1) {
        return -1;
    }
    for (int i = 0; i < svShellCommandsNum; ++i) {
        /* Attempt to find existing command */
        if (strcmp(commandName, svShellCommands[i].cmdName) == 0) {
            return -2;
        }
    }

    svShellCommands[svShellCommandsNum].cmdName = commandName;
    svShellCommands[svShellCommandsNum].cmdFunc = commandFunction;
    ++svShellCommandsNum;

    return 0;
}

void svShellRemoveCommand(const char *commandName) {
    for (int i = 0; i < svShellCommandsNum; ++i) {
        /* Find command to remove */
        if (strcmp(commandName, svShellCommands[i].cmdName) == 0) {
            /* If last command, merely reduce the number of shell commands and
             * let next command overwrite */
            if (i != svShellCommandsNum - 1) {
                /* Remove command by shifting every command after back one pos
                 */
                for (int j = i; j < svShellCommandsNum - 1; ++j) {
                    svShellCommands[j].cmdName = svShellCommands[j + 1].cmdName;
                    svShellCommands[j].cmdFunc = svShellCommands[j + 1].cmdFunc;
                }
            }

            --svShellCommandsNum;
            break;
        }
    }
}

int svShellExecute(const char *inputLine) {
    int result = 0;

    /* Clear output and error buffers */
    memset(svOutputBuffer, '\0', SV_SHELL_OUT_BUFFER_SIZE);
    memset(svErrorBuffer, '\0', SV_SHELL_OUT_BUFFER_SIZE);

    /* Validate input line */
    svTokenizerError tokenizerError;
    if (svTokenizerValidateInput(inputLine, &tokenizerError) != 0) {
        /* Error occurred */
        switch (tokenizerError.errorCode) {
        case -1: {
            snprintf(svErrorBuffer, SV_SHELL_OUT_BUFFER_SIZE, "%s%d.",
                     "String not closed at position ",
                     tokenizerError.characterPos);
            break;
        }
        default:
            break;
        }

        result = -1;
    } else {
        /* Copy input line into a buffer we can modify */
        size_t inputLineLength =
            strlen(inputLine) + 1; /* +1 for NULL-terminator */
        char *inputLineCopy = (char *)malloc(sizeof(char) * inputLineLength);
        memset(inputLineCopy, '\0', inputLineLength);
        snprintf(inputLineCopy, inputLineLength, "%s", inputLine);

        /** Allocate tokens array */
        int numTokens = svTokenizerGetNumTokens(inputLine, NULL) +
                        1; /* NULL for error out
                        * because we already
                        * validated input above */
        char **token = (char **)malloc(sizeof(char **) * numTokens);
        memset(token, '\0', sizeof(char *) * numTokens);
        svTokenizerTokenize(inputLineCopy, token, NULL);

        /* Validate token array */
        svCommandError commandError;
        if (svCommandsValidateTokenArray(token, &commandError) != 0) {
            /* Error occurred */
            switch (commandError.errorCode) {
            case -1:
                /* Two successive commands are separated by more than one
                 * command separator */
                snprintf(
                    svErrorBuffer, SV_SHELL_OUT_BUFFER_SIZE,
                    "Two successive commands are separated by more than one "
                    "command separator: %s %s %s",
                    token[commandError.tokenPos - 1],
                    token[commandError.tokenPos],
                    token[commandError.tokenPos + 1]);
                break;
            case -2:
                /* First token is a command separator */
                snprintf(svErrorBuffer, SV_SHELL_OUT_BUFFER_SIZE,
                         "First token is a command separator.");
                break;
            default:
                break;
            }

            result = -1;
        } else {
            /* Allocate space for commands, +1 for NULL command required. */
            int numCommands = svCommandsGetNum(token, NULL) + 1;
            svCommand *commands =
                (svCommand *)malloc(sizeof(svCommand) * numCommands);
            memset(commands, '\0', sizeof(svCommand) * numCommands);
            svCommandsMakeNullCommand(commands + numCommands - 1);

            svCommandsSeparate(token, commands, NULL);

            /* Process commands */
            /* For each command (ignoring NULL command),
             * calc argc, send argv, argc to matchin funcptr (if any) */
            for (int i = 0; i < numCommands - 1; ++i) {
                svCommand *command = commands + i;
                /* Find matching shell command */
                int found = 0;
                for (int j = 0; j < svShellCommandsNum; ++j) {
                    if (strcmp(command->argv[0], svShellCommands[j].cmdName) ==
                        0) {
                        found = 1;
                        /* Calc argc */
                        int argc = 0;
                        for (int k = 0; command->argv[k] != NULL; ++k) {
                            ++argc;
                        }

                        int result =
                            svShellCommands[j].cmdFunc(argc, command->argv);
                        if (result != 0) {
                            free(commands);
                            free(token);
                            free(inputLineCopy);

                            return -1;
                        }

                        break;
                    }
                }

                if (found == 0) {
                    snprintf(svErrorBuffer, SV_SHELL_OUT_BUFFER_SIZE,
                             "No command with name '%s'.", command->argv[0]);

                    free(commands);
                    free(token);
                    free(inputLineCopy);

                    return -1;
                }
            }

            free(commands);
        }

        free(token);
        free(inputLineCopy);
    }

    return result;
}

size_t svShellGetOutputBufferSize() {
    return ((strlen(svOutputBuffer) + 1) * sizeof(char));
}

void svShellGetOutputBuffer(void *outBuffer) {
    snprintf(outBuffer, svShellGetOutputBufferSize(), "%s", svOutputBuffer);
}

size_t svShellGetErrorBufferSize() {
    return ((strlen(svErrorBuffer) + 1) * sizeof(char));
}

void svShellGetErrorBuffer(void *outBuffer) {
    snprintf(outBuffer, svShellGetErrorBufferSize(), "%s", svErrorBuffer);
}

void svShellPrintOutputBuffer(const char *format, ...) {
    va_list argptr;
    va_start(argptr, format);
    /* Find current end of buffer */
    size_t endOffset = strlen(svOutputBuffer);
    vsnprintf(svOutputBuffer + endOffset, SV_SHELL_OUT_BUFFER_SIZE - endOffset,
              format, argptr);
    va_end(argptr);
}

void svShellPrintErrorBuffer(const char *format, ...) {
    va_list argptr;
    va_start(argptr, format);
    /* Find current end of buffer */
    size_t endOffset = strlen(svErrorBuffer);
    vsnprintf(svErrorBuffer + endOffset, SV_SHELL_OUT_BUFFER_SIZE - endOffset,
              format, argptr);
    va_end(argptr);
}
