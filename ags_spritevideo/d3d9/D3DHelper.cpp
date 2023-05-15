#if defined (WINDOWS_VERSION)

#include "D3DHelper.h"
#include <d3d9.h>
#include "Common.h"
#include "D3D9Factory.h"


IDirect3DTexture9* CreateTexture(unsigned char const* data, int width, int height, bool alpha)
{
    if (!GetD3D())
    {
        DBG("Device not available");
        return NULL;
    }

    DBG("Creating texture");
    IDirect3DTexture9* texture = NULL;

    // RGB or ARGB format
    D3DFORMAT format = D3DFMT_X8R8G8B8;

    if (alpha)
    {
        format = D3DFMT_A8R8G8B8;
    }

    // Create texture
    int result = GetD3D()->CreateTexture(width, height, 1,
        D3DUSAGE_DYNAMIC, format,
        D3DPOOL_DEFAULT, &texture, NULL);

    if (result != D3D_OK)
    {
        DBGF("ERROR: Couldn't create texture: %08x", result);
        return NULL;
    }

    DBG("OK");

    SetTextureData(texture, data, width, height);
    return texture;
}

bool SetTextureData(IDirect3DTexture9* texture, unsigned char const* data, int width, int height)
{
    //DBG( "Setting texture data" );

    // Lock texture for writing
    D3DLOCKED_RECT texRect;
    if (FAILED(texture->LockRect(0, &texRect, NULL, D3DLOCK_DISCARD)))
    {
        DBG("ERROR: LockRect failed");
        return false;
    }

    //DBG( "Copying data" );
    // Copy data rows
    int bytesPerPixel = 4;
    for (int y = 0; y < height; ++y)
    {
        unsigned char* pDest = (unsigned char*)texRect.pBits + y * texRect.Pitch;
        memcpy(pDest, data + y * width * bytesPerPixel, width * bytesPerPixel);
    }

    if (FAILED(texture->UnlockRect(0)))
    {
        DBG("ERROR: UnlockRect failed");
        return false;
    }

    //DBG( "OK" );
    return true;
}


IDirect3DTexture9* CreateTexture(unsigned char const* const* data, int width, int height, bool alpha)
{
    if (!GetD3D())
    {
        DBG("Device not available");
        return NULL;
    }

    DBG("Creating texture");
    IDirect3DTexture9* texture = NULL;

    // RGB or ARGB format
    D3DFORMAT format = D3DFMT_X8R8G8B8;

    if (alpha)
    {
        format = D3DFMT_A8R8G8B8;
    }

    // Create texture
    int result = GetD3D()->CreateTexture(width, height, 1,
        D3DUSAGE_DYNAMIC, format,
        D3DPOOL_DEFAULT, &texture, NULL);

    if (result != D3D_OK)
    {
        DBGF("ERROR: Couldn't create texture: %08x", result);
        return NULL;
    }

    DBG("OK");

    SetTextureData(texture, data, width, height);
    return texture;
}

bool SetTextureData(IDirect3DTexture9* texture, unsigned char const* const* data, int width, int height)
{
    //DBG( "Setting texture data" );

    // Lock texture for writing
    D3DLOCKED_RECT texRect;
    if (FAILED(texture->LockRect(0, &texRect, NULL, D3DLOCK_DISCARD)))
    {
        DBG("ERROR: LockRect failed");
        return false;
    }

    //DBG( "Copying data" );
    // Copy data rows
    int bytesPerPixel = 4;
    for (int y = 0; y < height; ++y)
    {
        unsigned char* pDest = (unsigned char*)texRect.pBits + y * texRect.Pitch;
        memcpy(pDest, data[y], width * bytesPerPixel);
    }

    if (FAILED(texture->UnlockRect(0)))
    {
        DBG("ERROR: UnlockRect failed");
        return false;
    }

    //DBG( "OK" );
    return true;
}

#endif // WINDOWS_VERSION
