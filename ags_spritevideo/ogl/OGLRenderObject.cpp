#include "OGLRenderObject.h"
#include <vector>
#include <glad/glad.h>
#include "Common.h"
#include "BaseObject.h"
#include "ImageHelper.h"

// TODO: move to a shared header
#if OPENGL_ES2
    #define GL_CLAMP GL_CLAMP_TO_EDGE
#endif


OGLCUSTOMVERTEX OGLRenderObject::defaultVertices[4]{};
ShaderProgram OGLRenderObject::defaultProgram;

static const auto default_vertex_shader_src = ""
#if OPENGL_ES2
"#version 100 \n"
#else
"#version 120 \n"
#endif
R"EOS(
uniform mat4 uMVPMatrix;

attribute vec2 a_Position;
attribute vec2 a_TexCoord;

varying vec2 v_TexCoord;

void main() {
  v_TexCoord = a_TexCoord;
  gl_Position = uMVPMatrix * vec4(a_Position.xy, 0.0, 1.0);
}

)EOS";


static const auto default_fragment_shader_src = ""
#if OPENGL_ES2
"#version 100 \n"
"precision mediump float; \n"
#else
"#version 120 \n"
#endif
R"EOS(
uniform sampler2D textID;
uniform vec4 rgba;

varying vec2 v_TexCoord;

void main() {
  vec4 src_col = texture2D(textID, v_TexCoord);
  gl_FragColor = vec4(src_col.x * rgba.x, src_col.y * rgba.y,
                      src_col.z * rgba.z, src_col.w * rgba.w);
}
)EOS";


bool CreateDefaultShader(ShaderProgram &prg)
{
    if (!CreateShaderProgram(prg, "Default", default_vertex_shader_src, default_fragment_shader_src)) return false;
    prg.MVPMatrix = glGetUniformLocation(prg.Program, "uMVPMatrix");
    prg.TextureId = glGetUniformLocation(prg.Program, "textID");
    prg.Color = glGetUniformLocation(prg.Program, "rgba");
    return true;
}

bool OGLRenderObject::CreateStaticData()
{
    // Default vertices
    defaultVertices[0].position.x = 0.0f;
    defaultVertices[0].position.y = 0.0f;
    defaultVertices[0].tu = 0.0;
    defaultVertices[0].tv = 0.0;

    defaultVertices[1].position.x = 1.0f;
    defaultVertices[1].position.y = 0.0f;
    defaultVertices[1].tu = 1.0;
    defaultVertices[1].tv = 0.0;

    defaultVertices[2].position.x = 0.0f;
    defaultVertices[2].position.y = -1.0f;
    defaultVertices[2].tu = 0.0;
    defaultVertices[2].tv = 1.0;

    defaultVertices[3].position.x = 1.0f;
    defaultVertices[3].position.y = -1.0f;
    defaultVertices[3].tu = 1.0;
    defaultVertices[3].tv = 1.0;

    // Shaders
    bool shaders = CreateDefaultShader(defaultProgram);

    return shaders;
}

OGLRenderObject::~OGLRenderObject()
{
    if (myTexture)
    {
        glDeleteTextures(1, &myTexture);
        myTexture = 0u;
    }
}

void OGLRenderObject::CreateTexture(int sprite_id, int bkg_num, const char *file)
{
    if (sprite_id >= 0)
    {
        BITMAP* bmp = GetAGS()->GetSpriteGraphic(sprite_id);
        myWidth = GetAGS()->GetSpriteWidth(sprite_id);
        myHeight = GetAGS()->GetSpriteHeight(sprite_id);
        myHasAlpha = GetAGS()->IsSpriteAlphaBlended(sprite_id) != 0;
        myTexWidth = myWidth;
        myTexHeight = myHeight;

        unsigned char** data = GetAGS()->GetRawBitmapSurface(bmp);

        myTexture = ::CreateTexture(data, myWidth, myHeight, myHasAlpha);

        if (!myTexture)
        {
            DBGF("Could not open sprite #%d", sprite_id);
        }
    }
    else if (bkg_num >= 0)
    {
        BITMAP* bmp = GetAGS()->GetBackgroundScene(bkg_num);
        int unused;
        GetAGS()->GetBitmapDimensions(bmp, &myWidth, &myHeight, &unused);
        myHasAlpha = false;
        myTexWidth = myWidth;
        myTexHeight = myHeight;

        unsigned char** data = GetAGS()->GetRawBitmapSurface(bmp);

        myTexture = ::CreateTexture(data, myWidth, myHeight);

        if (!myTexture)
        {
            DBGF("Could not open room background #%d", bkg_num);
        }
    }
    else if (file)
    {
        ImageInfo info;
        std::vector<unsigned char> data;
        if (LoadImage(file, data, info, /*rgba*/ true))
        {
            myWidth = info.Width;
            myHeight = info.Height;
            myTexWidth = myWidth;
            myTexHeight = myHeight;
            myHasAlpha = info.HasAlpha;
            myTexture = ::CreateTexture(&data[0], info.Width, info.Height, info.HasAlpha);
        }

        if (!myTexture)
        {
            DBGF("Could not create texture from file %s", file);
        }
    }
}

void OGLRenderObject::CreateTexture(const unsigned char* data, int width, int height, int bpp)
{
    if (myTexture == 0 || myTexWidth != width || myTexHeight != height)
    {
        if (myTexture > 0)
            glDeleteTextures(1, &myTexture);
        myTexture = ::CreateTexture(data, width, height);
        myWidth = width;
        myHeight = height;
        myTexWidth = width;
        myTexHeight = height;
        myHasAlpha = false; // CHECKME??
    }
    else
    {
        ::SetTextureData(myTexture, data, width, height);
    }
}

void OGLRenderObject::Render(const Point &pos, const PointF &scaling, float rotation,
    const PointF &anchorPos, const RGBA &rgba, int filter)
{
    //DBG("myRender::Render");
    const Screen *screen = GetScreen();

    float screenScaleX = 1.0;
    float screenScaleY = 1.0;
    if (screen->matrixValid)
    {
        float orthoSizeX = 2.f / screen->globalProj._11;
        float orthoSizeY = 2.f / screen->globalProj._22;
        screenScaleX = orthoSizeX / screen->width;
        screenScaleY = orthoSizeY / screen->height;
        //DBG("---ORTHO w = %f, h = %f", orthoSizeX, orthoSizeY);
    }
    else
    {
    }

    //DBGF("---RENDER screenScale: %f,%f", screenScaleX, screenScaleY);

    // World matrix, set position, anchor, rotation and scaling
    Matrix trans, scale, rot, anchor;
    //DBGF("---RENDER TRANS: %f,%f", pos.x - screenScaleX * screen->width / 2.f, pos.y - (1.f + 1.f - screenScaleY) * screen->height / 2.f);
    SetMatrix(&trans, pos.x - screenScaleX * screen->width / 2.f,
        pos.y - (1.f + 1.f - screenScaleY) * screen->height / 2.f,
        //((1.f + 1.f - screenScaleY) * screen->height / 2.f) - pos.y,
        1, 1);
    //DBGF("---RENDER SCALE: %f,%f", screenScaleX * myWidth * scaling.x, screenScaleY * myHeight * scaling.y);
    SetMatrix(&scale, 0, 0, screenScaleX * myWidth * scaling.x, screenScaleY * myHeight * scaling.y);
    //DBGF("---RENDER ROTATION: %f at %f,%f", rotation, -anchorPos.x, anchorPos.y);
    SetMatrix(&anchor, -anchorPos.x - 0.5f, anchorPos.y + 0.5f, 1.f, 1.f); // Mirror Y
    SetMatrixRotation(&rot, rotation * RADS_PER_DEGREE);

    Matrix world;
    // OpenGL multiplies matrixes in reverse:
    // first global and then self transform
    MatrixMulOGL(&world, &screen->globalProj, &screen->globalView);
    MatrixMulOGL(&world, &world, &screen->globalWorld);
    MatrixMulOGL(&world, &world, &trans);
    // FIXME: Origin is at the middle of the surface
    // perhaps pass from the engine as globalView?
    world._41 += 1.f;
    world._42 += 1.f;
    MatrixMulOGL(&world, &world, &rot);
    MatrixMulOGL(&world, &world, &scale);
    MatrixMulOGL(&world, &world, &anchor);


    // Scale texture coordinates
    /* CHECKME?
    float scaleU = myTexWidth / static_cast<float>(myWidth);
    float scaleV = myTexHeight / static_cast<float>(myHeight);
    */

    // Default shader
    const ShaderProgram &program = defaultProgram;
    glUseProgram(program.Program);
    glUniform1i(program.TextureId, 0);
    glUniform4f(program.Color, rgba.r, rgba.g, rgba.b, rgba.a);
    glUniformMatrix4fv(program.MVPMatrix, 1, GL_FALSE, world.marr);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, myTexture);

    if (filter == BaseObject::FILTER_LINEAR)
    {
        // Linear texture filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    else if (filter == BaseObject::FILTER_NEAREST)
    {
        // Nearest neighbor filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glEnableVertexAttribArray(0);
    GLint a_Position = glGetAttribLocation(program.Program, "a_Position");
    glVertexAttribPointer(a_Position, 2, GL_FLOAT, GL_FALSE, sizeof(OGLCUSTOMVERTEX), &(defaultVertices[0].position));

    glEnableVertexAttribArray(1);
    GLint a_TexCoord = glGetAttribLocation(program.Program, "a_TexCoord");
    glVertexAttribPointer(a_TexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(OGLCUSTOMVERTEX), &(defaultVertices[0].tu));

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glUseProgram(0); // disable shader
}
