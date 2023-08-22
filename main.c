#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <X11/Xlib.h>

int main() {
  Display *display = XOpenDisplay(NULL);

  if (display == NULL) {
    fprintf(stderr, "Display is null. Exiting.");
    return 1;
  }

  Atom wmProtocolsAtom = XInternAtom(display, "WM_PROTOCOLS", false);
  Atom wmDeleteWindowAtom = XInternAtom(display, "WM_DELETE_WINDOW", false);


  int screen = DefaultScreen(display);
  Window root = RootWindow(display, screen);
  Window win = XCreateSimpleWindow(display, root, 0, 0, 300, 300, 0, 0, 0);
  XSetWMProtocols(display, win, &wmDeleteWindowAtom, 1);
  XMapRaised(display, win);
  
  bool running = true;
  XEvent event;
  XSelectInput(display, win, SubstructureNotifyMask);

  while(running) {
    XNextEvent(display, &event);

    switch (event.type) {
      case ClientMessage: {
        if (event.xclient.message_type == wmProtocolsAtom && event.xclient.data.l[0] == wmDeleteWindowAtom) {
          running = false;
        }
      } break;
      default: {
      } break;
    }
  }

  XDestroyWindow(display, win);
  XCloseDisplay(display);
  return 0;
}
