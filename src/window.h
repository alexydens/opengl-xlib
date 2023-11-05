#if !defined(WINDOW_H)
#define WINDOW_H

/* ABC, my ANSI-compatible Base library for C */
#include <ABC/base.h>

/* X11, windowing library */
#include <X11/X.h>
#include <X11/Xlib.h>

/* OpenGL */
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>

/* A window */
typedef struct {
  /* X11 stuff */
  Display *dpy;       /* Display */
  Window  root;       /* Root window */
  Window  win;        /* Window handle */
  Atom    wm_destroy; /* For detecting window destruction */

  /* Properties */
  u32 width;
  u32 height;
  bool running;
  bool resized;

  /* Only used for OpenGL windows */
  GLXContext gl_context;
  bool opengl_window;
} WindowData;

/* Create a X11 window */
WindowData createWindow(const char* title, u32 x, u32 y, u32 w, u32 h);
/* Destroy a X11 window */
void destroyWindow(WindowData* window);
/* Finish using windows */
void windowingQuit();

/* Create a X11 window with an OpenGL context */
WindowData createWindowGL(const char* title, u32 x, u32 y, u32 w, u32 h);
/* Destroy a X11 window with an OpenGL context */
void destroyWindowGL(WindowData* window);
/* Make OpenGL window current */
void makeCurrent(WindowData* window);
/* Present render to screen */
void presentWindowGL(WindowData* window);

/* Process next event */
void processNextEvent(WindowData** ppWindows, u32 numWindows);

#endif /* WINDOW_H */
