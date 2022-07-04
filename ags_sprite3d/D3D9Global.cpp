#include "D3D9Global.h"
#include "Common.h"
#include "D3DHelper.h"

IDirect3DDevice9* d3dDevice = NULL;

IDirect3DDevice9* GetD3D()
{
    return d3dDevice;
}

void InitD3DGfx(void* data, Screen* screen)
{
    D3DPRESENT_PARAMETERS* params = (D3DPRESENT_PARAMETERS*)data;
    if (params->BackBufferFormat != D3DFMT_X8R8G8B8)
    {
        GetAGS()->AbortGame("32bit colour mode required.");
    }

    screen->backBufferWidth = params->BackBufferWidth;
    screen->backBufferHeight = params->BackBufferHeight;
    screen->bpp = 32;
}

void initGraphics()
{
}

void InitGfxDevice(void* data)
{
    if (!d3dDevice)
    {
        // Received Direct3D device pointer
        d3dDevice = (IDirect3DDevice9*)data;
        initGraphics();
    }
}

void SetScreenMatrixes(Screen* screen, float(*world)[16], float(*view)[16], float(*proj)[16])
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
        SetMatrixIdentity(reinterpret_cast<D3DMATRIX*>(&screen->globalWorld));
        SetMatrixIdentity(reinterpret_cast<D3DMATRIX*>(&screen->globalView));
        SetMatrixIdentity(reinterpret_cast<D3DMATRIX*>(&screen->globalProj));
        screen->matrixValid = false;
    }
}
