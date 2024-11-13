#include "file_manage.h"
#include "console.h"

#include <stdio.h>
#include <string.h>

#include "geom_errors.h"

extern const char *errorText;
extern int errorType;
char buffer[256];

static const char *errorInline(const char *error, const int line) {
    static char errorTemplate[15 + 5 + 64] = "Error in line ";
    sprintf(errorTemplate + 14, "%d: %s", line, error);
    return errorTemplate;
}

int load_src(const int argc, const char **argv) {
    if(argc == 1)
        return throwError(ERROR_NO_ARG_GIVEN, "Please give a file.");

    const char *filename = argv[1];

    FILE *file = fopen(filename, "r");
    if(file == NULL)
        return throwError(ERROR_CANNOT_OPEN_FILE, cannotOpenFileError(filename));

    int count = 1;
    while(fgets(buffer, 256, file)) {
        if(processCommand(buffer) != 0)
            return throwError(errorType, errorInline(errorText, count));
        ++count;
    }

    fclose(file);
    return 0;
}