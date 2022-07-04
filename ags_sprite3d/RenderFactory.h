#ifndef SPRITE3D_RENDERFACTORY_H
#define SPRITE3D_RENDERFACTORY_H

#include "Structures.h"
#include "RenderObject.h"

class RenderFactory
{
public:
    virtual void InitGfxDevice(void* data) = 0;
    virtual bool InitGfxMode(Screen* screen, void* data) = 0;
    virtual void SetScreenMatrixes(Screen* screen, float(*world)[16], float(*view)[16], float(*proj)[16]) = 0;
    virtual RenderObject* CreateRenderObject() = 0;
};

#endif // SPRITE3D_RENDERFACTORY_H
