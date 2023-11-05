/* ABC, my ANSI-compatible Base library for C */
#include <ABC/base.h>

/* Project headers */
#include "window.h"
#include <GLES3/gl3.h>

/* Window */
WindowData window;
WindowData* pWindows[1];

/* Vertex data */
f32 vertices[] = {
    -0.5f, -0.5f, 1.0f,
     0.5f, -0.5f, 1.0f,
     0.0f,  0.5f, 1.0f,
};
/* Vertex buffer object, vertex array object */
u32 vbo, vao;
/* Shader program, other shaders will be deleted before main loop */
u32 fragmentShader, vertexShader, shaderProgram;
/* For debugging shader compilation and linking */
i32 success;
char infoLog[512];

/* Shader source */
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

int main() {
  /* Create window */
  window = createWindowGL("Xlib Window", 160, 120, 1280, 720);
  pWindows[0] = &window;

  /* Create objects for mesh */
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(f32), NULL);
  glEnableVertexAttribArray(0);

  /* Create shader program */
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    printf("Error: Failed to compile vertex shader:\n%s\n", infoLog);
  }
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    printf("Error: Failed to compile fragment shader:\n%s\n", infoLog);
  }
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    printf("Error: Failed to link shader program:\n%s\n", infoLog);
  }

  while (window.running) {
    /* Process events */
    processNextEvent(pWindows, 1);
    if(window.running) {

    /* Clear window */
    if (window.resized) glViewport(0, 0, window.width, window.height);
    glClearColor(0.3f, 0.5f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Draw scene */
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(f32), NULL);
    glEnableVertexAttribArray(0);

    glUseProgram(shaderProgram);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    /* Present window */
    presentWindowGL(&window);
    
    }
  }

  /* Destroy mesh objects */
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(2, &vao);

  /* Destroy window */
  destroyWindowGL(&window);
  windowingQuit();
  return 0;
}
