#include "OGLFactory.h"
#if OPENGL_ES2
    #include <EGL/egl.h>
#endif
#include <glad/glad.h>
#include "Common.h"
#include "OGLRenderObject.h"


bool glInitialized = false;

void OGLFactory::InitGfxDevice(void* data)
{
    if (glInitialized)
        return; // already initialized

#if OPENGL_ES2
    if (!gladLoadGLES2Loader((GLADloadproc)eglGetProcAddress))
    {
        GetAGS()->AbortGame("Failed to load GLES2.");
        return;
    }
    if (!GLAD_GL_ES_VERSION_2_0)
    {
        GetAGS()->AbortGame("Plugin requires OpenGLES 2.0 or higher.");
        return;
    }
#else
    if (!gladLoadGL())
    {
        GetAGS()->AbortGame("Failed to load GL.");
        return;
    }
    if (!GLAD_GL_VERSION_2_0)
    {
        GetAGS()->AbortGame("Plugin requires OpenGL 2.0 or higher.");
        return;
    }
#endif

    if (!OGLRenderObject::CreateStaticData())
    {
        GetAGS()->AbortGame("Plugin failed to initialize starting OpenGL resources.");
        return;
    }

    glInitialized = true;
}

bool OGLFactory::InitGfxMode(Screen* screen, void* data)
{
    // This function only supposed to check the requested display mode
    // before it's actually set in the engine, so we cannot do anything
    // GL-related at this point yet.
    return true;
}

void OGLFactory::SetScreenMatrixes(Screen* screen, float(*world)[16], float(*view)[16], float(*proj)[16])
{
    if (world && view && proj)
    {
        memcpy(screen->globalWorld.marr, *world, sizeof(float[16]));
        memcpy(screen->globalView.marr, *view, sizeof(float[16]));
        memcpy(screen->globalProj.marr, *proj, sizeof(float[16]));
        screen->matrixValid = true;
    }
    else
    {
        SetMatrixIdentity(&screen->globalWorld);
        SetMatrixIdentity(&screen->globalView);
        SetMatrixIdentity(&screen->globalProj);
        screen->matrixValid = false;
    }
}

std::unique_ptr<RenderObject> OGLFactory::CreateRenderObject()
{
    return std::make_unique<OGLRenderObject>();
}
