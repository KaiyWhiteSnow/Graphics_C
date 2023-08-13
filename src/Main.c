#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <curses.h>

typedef struct {
    float x;
    float y;
} Point;

void drawCircle(Display *display, Drawable d, GC gc, float centerX, float centerY, float radius) {
    for (int angle = 0; angle < 360; angle++) {
        float radians = angle * 3.14159265358979323846 / 180.0;
        float entity_x = centerX + (float)(cosf(radians) * radius);
        float entity_y = centerY + (float)(sinf(radians) * radius);
        
        XDrawPoint(display, d, gc, entity_x, entity_y);
        XFlush(display);

        usleep(2000);
    }
}

int main() {
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    scrollok(stdscr, TRUE);

    Display *display;
    Window window;
    XEvent event;

    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Cannot open display\n");
        return 1;
    }

    int screen = DefaultScreen(display);
    unsigned long white = WhitePixel(display, screen);
    unsigned long black = BlackPixel(display, screen);

    window = XCreateSimpleWindow(display, RootWindow(display, screen), 10, 10, 400, 300, 2, black, white);

    XSelectInput(display, window, ExposureMask | KeyPressMask);
    XMapWindow(display, window);

    GC gc = XCreateGC(display, window, 0, NULL);

    Point start = {200, 150};
    char ch = '\0';

    while (1) {
        while (XPending(display) > 0) {
            XNextEvent(display, &event);
            if (event.type == Expose) {
                
            } else if (event.type == KeyPress) {
                char keyBuffer[8];
                KeySym keySym;
                XLookupString(&event.xkey, keyBuffer, sizeof(keyBuffer), &keySym, NULL);
                if (keyBuffer[0] == 'w' || keyBuffer[0] == 'W') { // Check for lowercase and uppercase 'W'
                    drawCircle(display, window, gc, start.x, start.y, 100.0f);
                }
            }
        }
    }

    XDestroyWindow(display, window);
    XFreeGC(display, gc);
    XCloseDisplay(display);

    endwin();
    return 0;
}
