#include "D3DHelper.h"
#include <d3dx9.h>
#include <cmath>
#include "Common.h"
#include "D3D9Factory.h"

void SetMatrix(D3DMATRIX* matrix, float tx, float ty, float sx, float sy)
{
    matrix->_11 = sx;
    matrix->_12 = 0.0;
    matrix->_13 = 0.0;
    matrix->_14 = 0.0;
    matrix->_21 = 0.0;
    matrix->_22 = sy;
    matrix->_23 = 0.0;
    matrix->_24 = 0.0;
    matrix->_31 = 0.0;
    matrix->_32 = 0.0;
    matrix->_33 = 1.0;
    matrix->_34 = 0.0;
    matrix->_41 = tx;
    matrix->_42 = ty;
    matrix->_43 = 0.0;
    matrix->_44 = 1.0;
}

void SetMatrixIdentity(D3DMATRIX* matrix)
{
    matrix->_11 = 1.0;
    matrix->_12 = 0.0;
    matrix->_13 = 0.0;
    matrix->_14 = 0.0;
    matrix->_21 = 0.0;
    matrix->_22 = 1.0;
    matrix->_23 = 0.0;
    matrix->_24 = 0.0;
    matrix->_31 = 0.0;
    matrix->_32 = 0.0;
    matrix->_33 = 1.0;
    matrix->_34 = 0.0;
    matrix->_41 = 0.0;
    matrix->_42 = 0.0;
    matrix->_43 = 0.0;
    matrix->_44 = 1.0;
}

void SetMatrixRotation(D3DMATRIX* matrix, float radians)
{
    float sin = std::sinf(radians);
    float cos = std::cosf(radians);

    matrix->_11 = cos;
    matrix->_12 = -sin;
    matrix->_13 = 0.0;
    matrix->_14 = 0.0;
    matrix->_21 = sin;
    matrix->_22 = cos;
    matrix->_23 = 0.0;
    matrix->_24 = 0.0;
    matrix->_31 = 0.0;
    matrix->_32 = 0.0;
    matrix->_33 = 1.0;
    matrix->_34 = 0.0;
    matrix->_41 = 0.0;
    matrix->_42 = 0.0;
    matrix->_43 = 0.0;
    matrix->_44 = 1.0;
}

void MatrixMultiply(D3DMATRIX* result, const D3DMATRIX* a, const D3DMATRIX* b)
{
    D3DMATRIX temp;

    for (int j = 0; j < 4; ++j)
    {
        for (int i = 0; i < 4; ++i)
        {
            temp.m[j][i] = a->m[j][0] * b->m[0][i] +
                a->m[j][1] * b->m[1][i] +
                a->m[j][2] * b->m[2][i] +
                a->m[j][3] * b->m[3][i];
        }
    }

    memcpy(result->m, temp.m, sizeof(D3DMATRIX::m));
}

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
    int result = D3DXCreateTexture(GetD3D(), width, height, 1,
        D3DUSAGE_DYNAMIC, format,
        D3DPOOL_DEFAULT, &texture);

    if (result != D3D_OK)
    {
        DBG("ERROR: Couldn't create texture: %08x", result);
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
    int result = D3DXCreateTexture(GetD3D(), width, height, 1,
        D3DUSAGE_DYNAMIC, format,
        D3DPOOL_DEFAULT, &texture);

    if (result != D3D_OK)
    {
        DBG("ERROR: Couldn't create texture: %08x", result);
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
