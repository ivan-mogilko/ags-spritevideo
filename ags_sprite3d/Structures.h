/**
 * @file Structures.h
 * @author Ahonen
 *
 * @brief Data structures.
 */

#ifndef SPRITE3D_STRUCTURES_H
#define SPRITE3D_STRUCTURES_H

#include <string.h> // memset

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

struct Matrix
{
    union
    {
        struct
        {
            float        _11, _12, _13, _14;
            float        _21, _22, _23, _24;
            float        _31, _32, _33, _34;
            float        _41, _42, _43, _44;

        };
        float m[4][4];
        float marr[16];
    };
};

struct RGBA
{
    union
    {
        struct
        {
            float r, g, b, a;
        };
        float rgba[4];
    };
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
    Matrix globalWorld;
    Matrix globalView;
    Matrix globalProj;

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

#endif // SPRITE3D_STRUCTURES_H
