#include "console.h"
#include "object.h"
#include "file_manage.h"
#include "graphical.h"
#include "geom_errors.h"
#include "board.h"

#include <string.h>
#include <stdlib.h>

extern Window *mainWindow, *consoleWindow;
extern const char *errorText;
extern int errorType;

static char strCmdLine[256] = {0};
static int curser = 0;


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

static void reflashConsole() {
    windowFill(consoleWindow, 0x88, 0x88, 0x88);
    if (strCmdLine[0] != '\0')
        drawText(consoleWindow, strCmdLine, (Point2i){10, 30}, 0x0e0e0e, 15);
    if (errorText != NULL)
        drawText(consoleWindow, errorText, (Point2i){10, 90}, 0xff0000, 15);
    showWindow(mainWindow);
}

static char *consoleGetLine() {
    static char buffer[256];

    while (1) {
        const char c = waitKey(0);
        // 鼠标回调
        while (strCmdLine[curser] != 0)
            ++curser;

        switch (c) {
            case 27: // ESC
                destroyWindow(mainWindow);
#ifdef _WIN32
            case -1: // 点击窗口叉叉（只有windows有效）
#endif
                return NULL;
            case '\b':
                if (curser != 0)
                    strCmdLine[--curser] = 0;
                break;
            case '\n':
            case '\r':
                memcpy(buffer, strCmdLine, curser + 1);
                return buffer;
            default:
                strCmdLine[curser++] = c;
        }

        reflashConsole();
    }
}

static inline void pushback(const char *src) {
    memcpy(strCmdLine + curser, src, 8);
}

static int splitArgs(char *buffer, const char **argv) {
    int argc = 0;
    int isOneWord = 1;
    while (1) {
        switch (*buffer) {
            case '\n':
                *buffer = 0;
            case '\0':
                return argc;
            case ' ':
                *buffer++ = 0;
                isOneWord = 1;
                break;
            default:
                if (isOneWord) {
                    argv[argc++] = buffer;
                    isOneWord = 0;
                }
                ++buffer;
        }
        if (argc == 16)
            return argc;
    }
}

int processCommand(char *buffer) {
    static const char *argv[16];
    const int argc = splitArgs(buffer, argv);
    if (argc == 0) return 0;

    resetError();
    switch (strhash64(argv[0])) {
        case STR_HASH64('c', 'r', 'e', 'a', 't', 'e', 0, 0):
            return create(argc, argv);
        case STR_HASH64('s', 'h', 'o', 'w', 0, 0, 0, 0):
            return show(argc, argv);
        case STR_HASH64('h', 'i', 'd', 'e', 0, 0, 0, 0):
            return hide(argc, argv);
        case STR_HASH64('l', 'o', 'a', 'd', '-', 's', 'r', 'c'):
            return load_src(argc, argv);
        case STR_HASH64('m', 'i', 'd', 'p', 'o', 'i', 'n', 't'):
            return midpoint(argc, argv);
        default:
            return throwError(ERROR_UNKOWN_COMMAND, unkownCommand(argv[0]));
    }
}

static void mouseCallback(const int event, const int x, const int y, const int flags, void *userdata) {
    switch (event) {
        case EVENT_LBUTTONDOWN:
            const GeomObject *obj = mouseSelect(x, y);
            if (obj == NULL)
                return;
            pushback((char *) &obj->id);
            reflashConsole();
        default:
            break;
    }
}

void console() {
    windowFill(consoleWindow, 0x88, 0x88, 0x88);
    showWindow(mainWindow);
    setMouseCallback(mainWindow, mouseCallback, NULL);

    while (1) {
        char *cmdLine = consoleGetLine();
        if (cmdLine == NULL) break;

        memset(strCmdLine, 0, curser);
        curser = 0;

        processCommand(cmdLine);
        reflashConsole();
    }
}
