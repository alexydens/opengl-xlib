#include "window.h"

static Display* dpy = NULL;

/* Create a X11 window */
WindowData createWindow(const char* title, u32 x, u32 y, u32 w, u32 h) {
  WindowData window;
  /* Get display */
  if (dpy == NULL) {
    window.dpy = XOpenDisplay(NULL);
    dpy = window.dpy;
  }
  else window.dpy = dpy;
  ASSERT(window.dpy != NULL);

  /* Get root window */
  window.root = DefaultRootWindow(window.dpy);

  /* Create window */
  window.win = XCreateSimpleWindow(
      window.dpy, window.root,
      x, y, w, h, 10,
      0, 0
  );
  ASSERT(window.win != 0);
  window.width = w;
  window.height = h;
  XMapWindow(window.dpy, window.win);
  XStoreName(window.dpy, window.win, title);
  /* To get an event when window resizes */
  XSelectInput(window.dpy, window.win,
      ExposureMask | KeyPressMask | StructureNotifyMask);
  /* To get an event when window closes */
  window.wm_destroy = XInternAtom(window.dpy, "WM_DELETE_WINDOW", 0);
  XSetWMProtocols(window.dpy, window.win, &window.wm_destroy, 1);

  /* Set some stuff */
  window.running = true;
  window.resized = true;
  window.opengl_window = false;

  return window;
}
/* Destroy a X11 window */
void destroyWindow(WindowData* window) {
  if (window->running) {
    XUnmapWindow(window->dpy, window->win);
    XDestroyWindow(window->dpy, window->win);
  }
}
/* Finish using windows */
void windowingQuit() {
  XCloseDisplay(dpy);
}

/* Create a X11 window with an OpenGL context */
WindowData createWindowGL(const char* title, u32 x, u32 y, u32 w, u32 h) {
  WindowData window;
  /* For OpenGL context */
  XVisualInfo* vi;
  Colormap cmap;
  XSetWindowAttributes swa;
  GLint att[] = {
    GLX_RGBA,
    GLX_DEPTH_SIZE, 24,
    GLX_DOUBLEBUFFER, None
  };

  /* Get display */
  if (dpy == NULL) {
    window.dpy = XOpenDisplay(NULL);
    dpy = window.dpy;
  }
  else window.dpy = dpy;
  ASSERT(window.dpy != NULL);

  /* Get root window */
  window.root = DefaultRootWindow(window.dpy);
  
  /* Choose visual */
  vi = glXChooseVisual(window.dpy, 0, att);
  ASSERT(vi != NULL);

  /* Create color map */
  cmap = XCreateColormap(window.dpy, window.root, vi->visual, AllocNone);
  /* Fill in some of set window attribs */
  swa.colormap = cmap;
  swa.event_mask = ExposureMask | KeyPressMask | StructureNotifyMask;

  /* Create window */
  window.win = XCreateWindow(
      window.dpy, window.root,
      x, y, w, h,
      0, vi->depth,
      InputOutput,
      vi->visual,
      CWColormap | CWEventMask,
      &swa
  );
  ASSERT(window.win != 0);
  window.width = w;
  window.height = h;
  XMapWindow(window.dpy, window.win);
  XStoreName(window.dpy, window.win, title);
  /* To get an event when window closes */
  window.wm_destroy = XInternAtom(window.dpy, "WM_DELETE_WINDOW", 0);
  XSetWMProtocols(window.dpy, window.win, &window.wm_destroy, 1);

  /* Create OpenGL context */
  window.gl_context = glXCreateContext(window.dpy, vi, NULL, GL_TRUE);
  glXMakeCurrent(window.dpy, window.win, window.gl_context);

  /* Set some stuff */
  window.running = true;
  window.resized = true;
  window.opengl_window = true;

  return window;
}
/* Destroy a X11 window with an OpenGL context */
void destroyWindowGL(WindowData* window) {
  if (window->running) {
    glXDestroyContext(window->dpy, window->gl_context);
    XUnmapWindow(window->dpy, window->win);
    XDestroyWindow(window->dpy, window->win);
  }
}
/* Make OpenGL window current */
void makeCurrent(WindowData* window) {
  glXMakeCurrent(window->dpy, window->win, window->gl_context);
}
/* Present render to screen */
void presentWindowGL(WindowData* window) {
  glXSwapBuffers(window->dpy, window->win);
}

/* Process next event */
void processNextEvent(WindowData** ppWindows, u32 numWindows) {
  XEvent xev;
  u32 i;
  for (i = 0; i < numWindows; i++)
    ppWindows[i]->resized = false;
  XNextEvent(ppWindows[0]->dpy, &xev);
  if (xev.type == ConfigureNotify) {
    WindowData* window = NULL;
    for (i = 0; i < numWindows; i++) {
      if (xev.xconfigure.window == ppWindows[i]->win) {
        window = ppWindows[i];
        window->resized = true;
        window->width = xev.xconfigure.width;
        window->height = xev.xconfigure.height;
      }
    }
  }
  if (xev.type == ClientMessage) {
    WindowData* window = NULL;
    for (i = 0; i < numWindows; i++) {
      if (xev.xclient.window == ppWindows[i]->win)
        window = ppWindows[i];
    }
    if (xev.xclient.data.l[0] == (i32)window->wm_destroy) {
      if (window->opengl_window) destroyWindowGL(window);
      else destroyWindow(window);
      window->running = false;
    }
  }
}
