#include "graphical.h"
#include "geometry.h"
#include <stdio.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

Window mainWindow;
const Point2i origin = {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 100};

char *console() {
    static char text[128] = {0};
    static int curser = 0;

    while (1){
        drawRect(mainWindow, (Point2i){0, WINDOW_HEIGHT - 100}, WINDOW_WIDTH, 100, 0x888888, -1);
        drawText(mainWindow, text, (Point2i){10, WINDOW_HEIGHT - 60}, 0x0e0e0e, 30);
        showWindow(mainWindow);

        const char c = waitKey(0);
        switch (c) {
            case 27:    // ESC
                return NULL;
            case '\b':
                if(curser != 0)
                    text[--curser] = 0;
                break;
            case '\n':
            case '\r':
                return text;
            default:
                text[curser++] = c;
        }
    }
}

void processInput(const char *input) {
    printf("%s\n", input);
}

int main(){
    graphicalInit();

    mainWindow = getNewWindow("Geometry", WINDOW_WIDTH, WINDOW_HEIGHT);

    while (1){
        const char *input = console();
        if(input == NULL)
            break;
        processInput(input);
    }

    destroyWindow(mainWindow);

    return 0;
}