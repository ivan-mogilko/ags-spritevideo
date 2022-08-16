#ifndef SPRITEVIDEO_OGLHELPER_H
#define SPRITEVIDEO_OGLHELPER_H

#include <glad/glad.h>

struct OGLVECTOR2D
{
    float x = 0.f;
    float y = 0.f;
};

struct OGLCUSTOMVERTEX
{
    OGLVECTOR2D position;
    float tu = 0.f;
    float tv = 0.f;
};

struct ShaderProgram
{
    GLuint Program = 0;

    GLuint MVPMatrix = 0;
    GLuint TextureId = 0;
    GLuint Color = 0;
};

unsigned CreateTexture(unsigned char const* data, int width, int height, bool alpha = false);
bool SetTextureData(unsigned texture, unsigned char const* data, int width, int height);
unsigned CreateTexture(unsigned char const* const* data, int width, int height, bool alpha = false);
bool SetTextureData(unsigned texture, unsigned char const* const* data, int width, int height);
bool CreateShaderProgram(ShaderProgram &prg, const char *name, const char *vertex_shader_src, const char *fragment_shader_src);
void DeleteShaderProgram(ShaderProgram &prg);

#endif // SPRITEVIDEO_OGLHELPER_H
