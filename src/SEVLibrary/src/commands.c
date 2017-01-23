#include <stdlib.h>
#include <string.h>

#include <sev/commands.h>

static void svCommandsBuildArgumentArray(const char *token[],
                                         svCommand *command) {
    /* Allocate space for argument array, note +1 for argument space and +1 for
     * NULL-terminator (argv must be NULL-terminated). */
    int first     = command->first;
    int last      = command->last;
    command->argv = (char **)malloc(sizeof(char *) * (last - first + 2));

    /* For each token in command, allocate space for token and copy it to the
     * appropriate position in the argument array */
    for (int i = 0; i < last - first + 1; ++i) {
        const char *str  = token[first + i];
        size_t len       = strlen(str) + 1; /* +1 for NULL terminator */
        command->argv[i] = (char *)malloc(sizeof(char) * len);
        memset(command->argv[i], '\0', len);
        strcpy(command->argv[i], str);
    }
    /* Make sure to NULL-terminate argument array */
    command->argv[last - first + 1] = NULL;
}

/**
 * \returns 1 if token is a command separator, 0 otherwise.
 */
static int svCommandsIsSeparator(char *token) {
    if (token == NULL) {
        return 0;
    }
    
    char *commandSeparators[] = {SV_SEP_SEQ, NULL};

    for (int i = 0; commandSeparators[i] != NULL; ++i) {
        if (strcmp(commandSeparators[i], token) == 0) {
            return 1;
        }
    }

    return 0;
}

int svCommandsValidateTokenArray(char *token[], svCommandError *error) {
    if (token == NULL || *token == NULL) {
        /* No input */
        return 0;
    }

    if (svCommandsIsSeparator(token[0])) {
        /* First token is a separator, return error */
        if (error != NULL) {
            error->errorCode = -2;
            error->tokenPos  = 0;
        }
        return -1;
    }

    char **it  = token + 1; /* Already know first token isn't a separator */
    char *last = '\0';
    int i      = 1;
    while (*it) {
        last = *it;
        ++it;
        if (it && svCommandsIsSeparator(last) && svCommandsIsSeparator(*it)) {
            /* Two successive command separators, return error */
            if (error != NULL) {
                error->errorCode = -1;
                error->tokenPos  = i;
            }
            return -1;
        }
        ++i;
    }

    /* Else, no error */
    return 0;
}

int svCommandsGetNum(char *token[], svCommandError *error) {
    if (svCommandsValidateTokenArray(token, error) != 0) {
        return -1;
    }
    
    if (token == NULL) {
        return 0;
    } else if (token && token[0] == NULL) {
        return 0;
    }

    int numCommands = 0;

    char **it = token;
    while (*it) {
        if (svCommandsIsSeparator(*it)) {
            ++numCommands;
        }
        ++it;
    }
    /* Above logic increments numCommands each time a separator is found, this
       works until the last element, which may or may not have a separator. */
    if (!(svCommandsIsSeparator(*(it - 1)))) {
        ++numCommands;
    }

    return numCommands;
}

int svCommandsSeparate(char *token[], svCommand command[],
                       svCommandError *error) {
    if (svCommandsValidateTokenArray(token, error) != 0) {
        return -1;
    }

    int c     = 0;
    int first = 0;
    int last  = 0;
    char *sep = SV_SEP_SEQ;

    int i     = 0;
    char **it = token;
    while (*it) {
        last = i;
        if (svCommandsIsSeparator(*it)) {
            sep = *it;

            command[c].first = first;
            command[c].last  = last - 1; /* Don't include separator */
            command[c].sep   = sep;
            ++c;

            /* Advance first indes */
            first = i + 1;
        } else if (*(it + 1) == NULL) {
            /* We're at last token and it's not a separator */
            /* Add a sequential separator */
            sep = SV_SEP_SEQ;

            command[c].first = first;
            command[c].last  = last;
            command[c].sep   = sep;
            ++c;
        }
        ++it;
        ++i;
    }

    int numCommands = c;
    for (int j = 0; j < numCommands; ++j) {
        svCommandsBuildArgumentArray((const char **)token, &command[j]);
    }

    return 0;
}

void svCommandsFreeContents(svCommand command[]) {
    svCommand *it = command;
    /* Iterate thru commands */
    while (it &&
           !(it->first == 0 && it->last == 0 && it->sep == NULL &&
             it->argv == NULL)) {
        if (it->argv != NULL) {
            /* Free each token */
            for (int i = 0; it->argv[i] != NULL; ++i) {
                free(it->argv[i]);
                it->argv[i] = NULL;
            }

            /* Free array */
            free(it->argv);
            it->argv = NULL;
        }

        ++it;
    }
}

void svCommandsMakeNullCommand(svCommand *command) {
    if (command != NULL) {
        command->first = 0;
        command->last  = 0;
        command->sep   = NULL;
        command->argv  = NULL;
    }
}
