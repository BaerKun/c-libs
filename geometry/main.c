#include "graphical.h"
#include "geometry.h"
#include <stdio.h>

Window mainWindow;
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;


char *console() {
    static char text[128] = {0};
    static int curser = 0;

    while (1){
        drawRect(mainWindow, (Point2f){-WINDOW_WIDTH / 2, 100 - WINDOW_HEIGHT / 2}, WINDOW_WIDTH, 100, 0x888888, -1);
        drawText(mainWindow, text, (Point2f){20 - WINDOW_WIDTH / 2, 60 - WINDOW_HEIGHT / 2}, 0x0e0e0e, 30);
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