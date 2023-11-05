/* ABC, my ANSI-compatible Base library for C */
#include <ABC/base.h>

/* Project headers */
#include "window.h"

WindowData window;
WindowData* pWindows[1];

int main() {
  window = createWindowGL("Xlib Window", 160, 120, 1280, 720);
  pWindows[0] = &window;

  while (window.running) {
    processNextEvent(pWindows, 1);
    if(window.running) {

    if (window.resized) glViewport(0, 0, window.width, window.height);
    glClearColor(0.3f, 0.5f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    presentWindowGL(&window);
    
    }
  }

  destroyWindowGL(&window);
  windowingQuit();
  return 0;
}
