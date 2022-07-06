#ifndef SPRITE3D_D3D9FACTORY_H
#define SPRITE3D_D3D9FACTORY_H

#if defined (WINDOWS_VERSION)

#include <d3d9.h>
#include "RenderFactory.h"

class D3D9Factory : public RenderFactory
{
public:
    void InitGfxDevice(void* data) override;
    bool InitGfxMode(Screen* screen, void* data) override;
    void SetScreenMatrixes(Screen* screen, float(*world)[16], float(*view)[16], float(*proj)[16]) override;
    std::unique_ptr<RenderObject> CreateRenderObject() override;
};

IDirect3DDevice9* GetD3D();

#endif // #if defined (WINDOWS_VERSION)

#endif // SPRITE3D_D3D9FACTORY_H
