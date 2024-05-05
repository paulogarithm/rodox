#include <stdio.h>
#include <X11/Xlib.h>
#include <stdlib.h>
#include <stdbool.h>

static void window_loop(Display *disp, Atom delete_win)
{
    XEvent ev = {.type = 0};
    bool exited = false;

    while (!exited) {
        XNextEvent(disp, &ev);
        switch (ev.type) {
            case ClientMessage:
            if (ev.xclient.data.l[0] == delete_win) {
                exited = true;
                XDestroyWindow(ev.xclient.display, ev.xclient.window);
            }
            break;
        }
    }
}

int main(void)
{
    Display *disp = XOpenDisplay(NULL);
    Window root = 0;
    Window window = 0;
    Atom delete_win = 0;

    if (disp == NULL)
        return EXIT_FAILURE;
    root = DefaultRootWindow(disp);
    if (root == None)
        return EXIT_FAILURE;
    window = XCreateSimpleWindow(disp, root, 0, 0, 800, 600, 0, 0, 0xffffff);
    if (window == None)
        return EXIT_FAILURE;
    XMapWindow(disp, window);
    delete_win = XInternAtom(disp, "WM_DELETE_WINDOW", false);
    XSetWMProtocols(disp, window, &delete_win, 1);
    window_loop(disp, delete_win);
    XCloseDisplay(disp);
    return 0;
}
