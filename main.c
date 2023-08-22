#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h>
#include <X11/cursorfont.h>


void select_window(Display *display, Window root) {
  Cursor cursor = XCreateFontCursor(display, XC_hand1);
  int status = XGrabPointer(display, root, false, ButtonReleaseMask, GrabModeSync, GrabModeAsync, root, cursor, CurrentTime);

  if (status != GrabSuccess) {
    fprintf(stderr, "Can't grab the mouse.\n");
    exit(1);
  }

  XEvent event;
  Window win = 0L;
  XAllowEvents(display, SyncPointer, CurrentTime);
  XWindowEvent(display, root, ButtonReleaseMask, &event);

  switch (event.type) {
    case ButtonRelease: {
        fprintf(stdout, "ButtonRelease");
	      win = event.xbutton.subwindow;
    } break;
    default: {
    } break;
  }

  XUngrabPointer(display, CurrentTime);
  fprintf(stdout, "Selected window: %d\n", win);
}

int main() {
  Display *display = XOpenDisplay(NULL);

  if (display == NULL) {
    fprintf(stderr, "Display is null. Exiting.\n");
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

  select_window(display, root);

  XCloseDisplay(display);
  return 0;
}
