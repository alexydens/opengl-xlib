#if !defined(TEXTURE_H)
#define TEXTURE_H

/* ABC, my ANSI-compatible Base library for C */
#include <ABC/base.h>

/* OpenGL */
#include <GLES3/gl3.h>

/* SDL2 image for image loading (ugh) */
#include <SDL2/SDL_image.h>

/* Shaders */
#include "shader.h"

/* Needs malloc() and free() */
#include <stdlib.h>

/* Texture structure */
typedef struct {
  u32* textures;
  u32 numTextures;
} Texture;

/* Create a texture */
Texture createTexture(
    const char** fileNames,
    const char** shaderNames,
    u32 numTextures,
    ShaderHandle shader);

/* Destroy a texture */
void destroyTexture(Texture* texture);

/* Bind texture */
void bindTexture(Texture* texture);

#endif
