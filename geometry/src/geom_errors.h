#ifndef GEOM_ERRORS_H
#define GEOM_ERRORS_H

typedef enum {
    ERROR_HELP=1,
    ERROR_UNKOWN_COMMAND,
    ERROR_NO_ARG_GIVEN,
    ERROR_NOT_ENOUGH_ARG,
    ERROR_INVALID_ARG,
    ERROR_NOT_FOUND_OBJECT,
    ERROR_CANNOT_OPEN_FILE
} GeomErrorType;

const char *objectNotFound(const char *name);

const char *unkownCommand(const char *cmd);

const char *noArgGiven(const char *cmd);

const char *cannotOpenFileError(const char *filename);

void throwError(GeomErrorType type, const char *text);

void resetError();

#endif //GEOM_ERRORS_H
