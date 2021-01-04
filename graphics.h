/* graphics.h */
#ifndef GRAPHICS_H
#define GRAPHICS_H

class Graphics
{
public:
    static Display *display;
    static int screen;
    static int depth;
    static Window win;

public:
    Graphics();
    ~Graphics();

    static int createWindow(char *title, int width, int height);
    static void closeWindow(void);
    static void drawRect(x, y, width, height);
}
#endif
