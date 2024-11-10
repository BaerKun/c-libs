#include "geom_errors.h"

#include <string.h>

const char *errorText = NULL;
int errorType = 0;


static void strmove8(char *dst, const char *src) {
    for (int i = 0; i < 8 && *src != '\0'; ++i) {
        *dst = *src;
        ++dst, ++src;
    }
    *dst = '\0';
}

const char *objectNotFound(const char *name) {
    static char error[32] = "Object not found: ";
    strmove8(error + 18, name);
    return error;
}

const char *unkownCommand(const char *cmd) {
    static char error[32] = "Unknown command: ";
    strmove8(error + 17, cmd);
    return error;
}

const char *cannotOpenFileError(const char *filename) {
    static char errorTemplate[19 + 64] = "Cannot open file: ";
    strncpy(errorTemplate + 18, filename, 64);
    return errorTemplate;
}

const char *noArgGiven(const char *cmd) {
    static char errorTemplate[24 + 8] = "No arguments given for ";
    strmove8(errorTemplate + 23, cmd);
    return errorTemplate;
}

void throwError(const GeomErrorType type, const char *text) {
    errorType = type;
    errorText = text;
}

void resetError() {
    errorType = 0;
    errorText = NULL;
}
