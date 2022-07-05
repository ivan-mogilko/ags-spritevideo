#ifndef SPRITE3D_D3DHELPER_H
#define SPRITE3D_D3DHELPER_H

#include <d3d9.h>

struct Vertex
{
    float x, y, z;      // Position
    float nx, ny, nz;   // Normal
    D3DCOLOR color;		// Color
    float tu, tv;       // Texture coords
    float tu2, tv2;     // Second texture coords
};

DWORD const VERTEX_FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_TEX2;

Vertex const DEFAULT_QUAD[4] =
{
    { -0.5f, -0.5f, 0.f, 0.f, 0.f, -1.f, D3DCOLOR_RGBA(1, 1, 1, 1), 0.f, 1.f, 0.f, 1.f },
    { 0.5f, -0.5f, 0.f, 0.f, 0.f, -1.f, D3DCOLOR_RGBA(1, 1, 1, 1), 1.f, 1.f, 1.f, 1.f },
    { 0.5f, 0.5f, 0.f, 0.f, 0.f, -1.f, D3DCOLOR_RGBA(1, 1, 1, 1), 1.f, 0.f, 1.f, 0.f },
    { -0.5f, 0.5f, 0.f, 0.f, 0.f, -1.f, D3DCOLOR_RGBA(1, 1, 1, 1), 0.f, 0.f, 0.f, 0.f }
};

// Texture creation
IDirect3DTexture9* CreateTexture( unsigned char const* data, int width, int height, bool alpha = false );
bool SetTextureData( IDirect3DTexture9* texture, unsigned char const* data, int width, int height );
IDirect3DTexture9* CreateTexture( unsigned char const* const* data, int width, int height, bool alpha = false );
bool SetTextureData( IDirect3DTexture9* texture, unsigned char const* const* data, int width, int height );

#endif // SPRITE3D_D3DHELPER_H
