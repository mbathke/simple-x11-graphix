/* xgrafik.c */
#include <stdio.h>
#include <stdlib.h>
#include <X11/keysym.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "my_delay.h"
#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 320

/* function prototypes */
static int create_window(void);
static void process_event(XEvent report);
static void eventloop(void);
static void close_window(void);
extern int delay(int i);
extern int time_diff(void);
static void draw_graphics(void);

/* global variables */
static unsigned int width, height;
static Display *display;
static int screen;
static int depth;
static Window win;

/* function creates a window */
static int create_window(void)
{
    char *window_name = "Einfachen Grafik-Primitiven";
    char *icon_name = "window";
    static XSizeHints size_hints;
    Window rootwin;

    width = WINDOW_WIDTH;
    height = WINDOW_HEIGHT;

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

static void close_window(void)
{
    XCloseDisplay(display);
}

static void eventloop(void)
{
    XEvent xev;
    int num_events;

    XFlush(display);
    num_events = XPending(display);
    while ((num_events != 0)) {
        num_events--;
        XNextEvent(display, &xev);
        process_event(xev);
    }
}

static void process_event(XEvent report)
{
    switch(report.type) {
        case Expose:    /* Expose event => draw image */
            if (report.xexpose.count == 0) {
                draw_graphics();
            }
            break;
        case KeyPress:
            printf("Tastaturaktion\n");
            break;
        case ButtonPressMask:
            printf("Mausaktion\n");
            break;
        default:
            break;
    }
}

static void draw_graphics()
{
    GC mygc;                        /* new: graphic context */
    unsigned long black, white;     /* numbers of the colors */
    int i;                          /* good for everything */
    XPoint points[50];              /* an amount of points */
    XPoint poly[4];                 /* 4 points for polygon */

    /* occupy some fields */
    /* draw a slope with points */
    for (i = 0; i < 50; i++) {
        points[i].x = 470 + i;
        points[i].y = 200 + i;
    }

    /* closed triangle */
    poly[0].x = 200;
    poly[0].y = 200;
    poly[1].x = 300;
    poly[1].y = 300;
    poly[2].x = 400;
    poly[2].y = 200;
    poly[3].x = 200;
    poly[3].y = 200;

    /* draw window on top */
    XMapRaised(display, win);
    /* get color codes */
    black = BlackPixel(display, screen);
    white = WhitePixel(display, screen);

    /* create graphical context */
    mygc = XCreateGC(display, win, 0,  0);
    XSetForeground(display, mygc, white);
    XSetBackground(display, mygc, black);
    /* clear everything */
    XClearWindow(display, win);

    /* draw a rectangle */
    XDrawRectangle(display, win, mygc, 10, 50, 50, 50);
    /* fill rectangle with color */
    XFillRectangle(display, win, mygc, 110, 50, 50, 50);
    /* draw a circle */
    XDrawArc(display, win, mygc, 210, 50, 50, 50, 0, 360 * 64);
    /* fill a circle */
    XFillArc(display, win, mygc, 310, 50, 50, 50, 0, 360 * 64);
    /* polygon */
    XFillPolygon(display, win, mygc, poly, 4, 0, CoordModeOrigin);
    /* slope */
    XDrawPoints(display, win, mygc, points, 50, CoordModeOrigin);
    /* clear buffer */
    XFlush(display);
    /* release gc */
    XFreeGC(display, mygc);
}

int main(int argc, char **argv)
{
    int quit = 0;
    int pause = 0;
    int td;

    create_window();
    gettimeofday(&st, NULL);
    while (!quit) {
        gettimeofday(&rt, NULL);
        td = time_diff();
        pause = delay(FRAME_LEN - td + pause);
        st = rt;
        eventloop();
    }

    close_window();
    return EXIT_SUCCESS;
}
