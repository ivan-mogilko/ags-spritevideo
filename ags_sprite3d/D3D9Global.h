#ifndef SPRITE3D_D3D9GLOBAL_H
#define SPRITE3D_D3D9GLOBAL_H

#include <d3d9.h>
#include "Structures.h"

IDirect3DDevice9* GetD3D();
void InitD3DGfx(void* data, Screen* screen);
void InitGfxDevice(void* data);
void SetScreenMatrixes(Screen* screen, float (*world)[16], float (*view)[16], float (*proj)[16]);

#endif
