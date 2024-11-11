#include "geom_errors.h"

#include <stdio.h>
#include <string.h>

const char *errorText = NULL;
int errorType = 0;


const char *objectNotFound(const char *name) {
    static char error[32] = "Object not found: ";
    memcpy(error + 18, name, 8);
    return error;
}

const char *unkownCommand(const char *cmd) {
    static char error[32] = "Unknown command: ";
    memcpy(error + 17, cmd, 8);
    return error;
}

const char *cannotOpenFileError(const char *filename) {
    static char errorTemplate[19 + 64] = "Cannot open file: ";
    strncpy(errorTemplate + 18, filename, 64);
    return errorTemplate;
}

const char *noArgGiven(const char *cmd) {
    static char errorTemplate[24 + 8] = "No arguments given for ";
    memcpy(errorTemplate + 23, cmd, 8);
    return errorTemplate;
}

const char *notEnoughArg(const char *cmd) {
    static char errorTemplate[26 + 8] = "Not enough arguments for ";
    memcpy(errorTemplate + 25, cmd, 8);
    return errorTemplate;
}

const char *invalidArg(const char *arg, const char *tips) {
    static char errorTemplate[9 + 64] = "Invalid ";
    if(tips == NULL)
        sprintf(errorTemplate + 8, "%s argument", arg);
    else
        sprintf(errorTemplate + 8, "%s argument. %s", arg, tips);

    return errorTemplate;
}

const char *unkownArg(const char *arg) {
    static char errorTemplate[19 + 16] = "Unknown argument: ";
    strncpy(errorTemplate + 18, arg, 16);
    return errorTemplate;
}

int throwError(const GeomErrorType type, const char *text) {
    errorType = type;
    errorText = text;
    return type;
}

void resetError() {
    errorType = 0;
    errorText = NULL;
}
