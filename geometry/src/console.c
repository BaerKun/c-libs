#include "console.h"

#include <object.h>
#include <stdio.h>

#include "graphical.h"

extern Window *mainWindow, *consoleWindow;

uint64_t strhash64(const char *str) {
    uint64_t hash = 0;
    char *p = (char *) &hash;

    for (int i = 0; i < 8 && *str != 0; ++i) {
        *p = *str;
        ++p, ++str;
    }
    return hash;
}

int strtobool(const char *str, const char **endptr) {
    switch (strhash64(str)) {
        case STR_HASH64('t', 'r', 'u', 'e', 0, 0, 0, 0):
            *endptr = str + 4;
            return 1;
        case STR_HASH64('f', 'a', 'l', 's', 'e', 0, 0, 0):
            *endptr = str + 5;
            return 0;
        default:
            *endptr = str;
            return 0;
    }
}

static void printError(const char *error) {
    drawText(consoleWindow, error, (Point2i){10, 90}, 0xff0000, 20);
}

static int splitArgs(const char *ptrCmdLine, char **argv) {
    static char buffer[256] = {0};

    char *ptrBuffer = buffer;
    int argc = 0;

    while (1) {
        while (*ptrCmdLine == ' ')
            ++ptrCmdLine;
        if (*ptrCmdLine == '\0') return argc;

        argv[argc++] = ptrBuffer;
        while (*ptrCmdLine != ' ') {
            *ptrBuffer++ = *ptrCmdLine++;
            if (*ptrCmdLine == '\0') {
                *ptrBuffer = '\0';
                return argc;
            }
        }
        *ptrBuffer++ = '\0';
        if (argc == 16) return argc;
    }
}

static char *consoleGetLine() {
    static char strCmdLine[256] = {0};
    static int curser = 0;

    while (1) {
        const char c = waitKey(0);
        switch (c) {
            case 27: // ESC
                return NULL;
            case '\b':
                if (curser != 0)
                    strCmdLine[--curser] = 0;
                break;
            case '\n':
            case '\r':
                return strCmdLine;
            default:
                strCmdLine[curser++] = c;
        }

        windowFill(consoleWindow, 0x88, 0x88, 0x88);
        drawText(consoleWindow, strCmdLine, (Point2i){10, 30}, 0x0e0e0e, 20);
        showWindow(mainWindow);
    }
}

static void processCommand(const char *cmdLine) {
    static char *argv[16];

    const int argc = splitArgs(cmdLine, argv);
    if (argc == 0) return;

    char *error;
    switch (strhash64(argv[0])) {
        case STR_HASH64('c', 'r', 'e', 'a', 't', 'e', 0, 0):
            error = create(argc, argv);
            break;
        case STR_HASH64('s', 'h', 'o', 'w', 0, 0, 0, 0):
            error = show(argc, argv);
            break;
        case STR_HASH64('h', 'i', 'd', 'e', 0, 0, 0, 0):
            error = hide(argc, argv);
            break;
        default:
            error = "Unknown command";
            break;
    }

    if (error != NULL)
        printError(error);
    showWindow(mainWindow);
}

void console() {
    windowFill(consoleWindow, 0x88, 0x88, 0x88);
    showWindow(mainWindow);

    while (1) {
        const char *cmdLine = consoleGetLine();
        if (cmdLine == NULL) return;

        processCommand(cmdLine);
    }
}
