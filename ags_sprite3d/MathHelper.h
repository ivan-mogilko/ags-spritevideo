#ifndef SPRITE3D_MATHHELPER_H
#define SPRITE3D_MATHHELPER_H

float const RADS_PER_DEGREE = 3.14159265f / 180.f;

struct Point
{
    int x = 0, y = 0;

    Point() = default;
    Point(int x, int y) : x(x), y(y) {}
};

struct PointF
{
    float x = 0.f, y = 0.f;

    PointF() = default;
    PointF(float x, float y) : x(x), y(y) {}
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

    Matrix() = default;
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

    RGBA() = default;
};


// Matrix functions
void SetMatrix(Matrix* matrix, float tx, float ty, float sx, float sy);
void SetMatrixIdentity(Matrix* matrix);
void SetMatrixRotation(Matrix* matrix, float radians);
void MatrixMulD3D(Matrix* result, const Matrix* ma, const Matrix* mb);
void MatrixMulOGL(Matrix* result, const Matrix* ma, const Matrix* mb);

#endif // SPRITE3D_MATHHELPER_H
