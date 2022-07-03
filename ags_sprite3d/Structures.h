/**
 * @file Structures.h
 * @author Ahonen
 *
 * @brief Data structures.
 */

#ifndef D3D_STRUCTURES_H
#define D3D_STRUCTURES_H


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
	{ -0.5f, -0.5f, 0.f, 0.f, 0.f, -1.f, D3DCOLOR_RGBA( 1, 1, 1, 1 ), 0.f, 1.f, 0.f, 1.f },
    { 0.5f, -0.5f, 0.f, 0.f, 0.f, -1.f, D3DCOLOR_RGBA( 1, 1, 1, 1 ), 1.f, 1.f, 1.f, 1.f },
    { 0.5f, 0.5f, 0.f, 0.f, 0.f, -1.f, D3DCOLOR_RGBA( 1, 1, 1, 1 ), 1.f, 0.f, 1.f, 0.f },
    { -0.5f, 0.5f, 0.f, 0.f, 0.f, -1.f, D3DCOLOR_RGBA( 1, 1, 1, 1 ), 0.f, 0.f, 0.f, 0.f }
};

struct Point
{
    int x, y;

    Point( int x, int y ): x( x ), y( y ) {}
};

struct PointF
{
    float x, y;

    PointF( float x, float y ): x( x ), y( y ) {}
};

struct Screen
{
    int width;
    int height;
    int bpp;
    Point viewport;
    int gameSpeed;
    float frameDelay;
	int backBufferWidth;
	int backBufferHeight;

    // Render stage transform matrixes
    bool matrixValid;
    D3DMATRIX globalWorld;
    D3DMATRIX globalView;
    D3DMATRIX globalProj;

    Screen():
        width( 0 ),
        height( 0 ),
        bpp( 0 ),
        viewport( 0, 0 ),
        gameSpeed( 40 ),
        frameDelay( 1.f / 40 ),
		backBufferWidth( 0 ),
		backBufferHeight( 0 )
    {
        matrixValid = false;
        memset(&globalWorld, 0, sizeof(float[16]));
        memset(&globalView, 0, sizeof(float[16]));
        memset(&globalProj, 0, sizeof(float[16]));
    }

    Point FromRoom( Point pt ) const
    {
        return Point( pt.x + viewport.x, height - pt.y - viewport.y );
    }

    Point FromScreen( Point pt ) const
    {
        return Point( pt.x, height - pt.y );
    }
};

#endif