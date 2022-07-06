#if defined (WINDOWS_VERSION)

#include "D3D9Factory.h"
#include "Common.h"
#include "D3D9RenderObject.h"

IDirect3DDevice9* d3dDevice = NULL;

IDirect3DDevice9* GetD3D()
{
    return d3dDevice;
}

void D3D9Factory::InitGfxDevice(void* data)
{
    if (!d3dDevice)
    {
        // Received Direct3D device pointer
        d3dDevice = (IDirect3DDevice9*)data;
    }
}

bool D3D9Factory::InitGfxMode(Screen* screen, void* data)
{
    D3DPRESENT_PARAMETERS* params = (D3DPRESENT_PARAMETERS*)data;
    if (params->BackBufferFormat != D3DFMT_X8R8G8B8)
    {
        GetAGS()->AbortGame("32bit colour mode required.");
        return false;
    }
    return true;
}

void D3D9Factory::SetScreenMatrixes(Screen* screen, float(*world)[16], float(*view)[16], float(*proj)[16])
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

std::unique_ptr<RenderObject> D3D9Factory::CreateRenderObject()
{
    return std::make_unique<D3D9RenderObject>();
}

#endif // WINDOWS_VERSION
