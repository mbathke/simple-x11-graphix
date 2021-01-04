/* graphics.cpp */

#include "graphics.h"

Graphics::Graphics() {};
~Graphics::Graphics() {};

static int Graphics::createWindow(char *title, int width, int height)
{
    char *icon_name = "window";
    static XSizeHints size_hints;
    Window rootwin;

    /* open x session */
    display = XOpenDisplay(NULL);

    /* error checks */
    if (display == NULL) {
        printf("Verbindung zum X-Server fehlgeschlagen!\n");
        exit(EXIT_FAILURE);
    }

    /* default screen of a display */
    screen = XDefaultScreen(display);

    /* default depth of the screen - count of layers */
    depth = XDefaultDepth(display, screen);
    rootwin = RootWindow(display, screen);
    win = XCreateSimpleWindow(display, rootwin, 10, 10,
                              width, height, 5,
                              WhitePixel(display, screen),
                              BlackPixel(display, screen));
    size_hints.flags = PSize | PMinSize | PMaxSize;
    size_hints.min_width = width;
    size_hints.max_width = width;
    size_hints.min_height = height;
    size_hints.max_height = height;
    XSetStandardProperties(display, win, window_name, icon_name,
                           None, 0, 0, &size_hints);
    XSelectInput(display, win,
                 ButtonPressMask | KeyPressMask | ExposureMask);
    XMapWindow(display, win);
    return 1;
}
