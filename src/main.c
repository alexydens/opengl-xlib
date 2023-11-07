/* ABC, my ANSI-compatible Base library for C */
#include <ABC/base.h>

/* Project headers */
#include "window.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"

/* OpenGL */
#include <GLES3/gl3.h>

/* Window */
WindowData window;
WindowData* pWindows[1];

/* Vertex data */
f32 vertices[] = {
     0.5f,  0.5f, 0.0f,   1.0f, 1.0f,
     0.5f, -0.5f, 0.0f,   1.0f, 0.0f,
    -0.5f,  0.5f, 0.0f,   0.0f, 1.0f,

     0.5f, -0.5f, 0.0f,   1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,
    -0.5f,  0.5f, 0.0f,   0.0f, 1.0f,
};

/* Mesh */
MeshCreateInfo meshCreateInfo;
u32 attribs[] = { 3, 2 };
u32 numAttribs = 2;
Mesh mesh;

/* Shader */
ShaderHandle shader;

/* Texture */
Texture texture;
const char* t_paths[] = { "assets/basecolor.jpg" };
const char* t_names[] = { "tex" };

int main() {
  /* Create window */
  window = createWindowGL("Xlib Window", 160, 120, 1280, 720);
  pWindows[0] = &window;

  /* Mesh creation */
  meshCreateInfo.vertices = vertices;
  meshCreateInfo.numVertices = 6;
  meshCreateInfo.attribs = attribs;
  meshCreateInfo.numAttribs = numAttribs;
  mesh = createMesh(meshCreateInfo);

  /* Shader creation */
  shader = createShader("assets/test.vert", "assets/test.frag");

  /* Texture creation */
  texture = createTexture(t_paths, t_names, 1, shader);

  while (window.running) {
    /* Process events */
    processNextEvent(pWindows, 1);
    if(window.running) {

    /* Clear window */
    if (window.resized) glViewport(0, 0, window.width, window.height);
    glClearColor(0.3f, 0.5f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Draw scene */
    bindShader(shader);
    bindTexture(&texture);
    drawMesh(&mesh);

    /* Present window */
    presentWindowGL(&window);
    
    }
  }

  /* Destroy texture */
  destroyTexture(&texture);
  /* Destroy shader */
  destroyShader(shader);
  /* Destroy mesh */
  destroyMesh(&mesh);
  /* Destroy window */
  destroyWindowGL(&window);
  windowingQuit();
  return 0;
}
