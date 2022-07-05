#include "MathHelper.h"
#include <cmath>
#include <cstring>


void SetMatrix(Matrix* matrix, float tx, float ty, float sx, float sy)
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

void SetMatrixIdentity(Matrix* matrix)
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

void SetMatrixRotation(Matrix* matrix, float radians)
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

void MatrixMulD3D(Matrix* result, const Matrix* a, const Matrix* b)
{
    Matrix temp;

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

    memcpy(result->m, temp.m, sizeof(Matrix::m));
}

void MatrixMulOGL(Matrix* result, const Matrix* a, const Matrix* b)
{
    Matrix temp;
    for (int j = 0; j < 4; ++j)
    {
        for (int i = 0; i < 4; ++i)
        {
            temp.m[i][j] = a->m[0][j] * b->m[i][0] +
                a->m[1][j] * b->m[i][1] +
                a->m[2][j] * b->m[i][2] +
                a->m[3][j] * b->m[i][3];
        }
    }
    memcpy(result->marr, temp.marr, sizeof(Matrix::marr));
}
