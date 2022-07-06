#ifndef SPRITE3D_D3D9RENDEROBJECT_H
#define SPRITE3D_D3D9RENDEROBJECT_H

#if defined (WINDOWS_VERSION)

#include <d3d9.h>
#include "MathHelper.h"
#include "RenderObject.h"

class D3D9RenderObject : public RenderObject
{
public:
    D3D9RenderObject() = default;
    ~D3D9RenderObject() override;

    void CreateTexture(int sprite_id, int bkg_num, const char *file) override;
    void CreateTexture(const unsigned char* data, int width, int height, int bpp) override;
    void Render(const Point &pos, const PointF &scaling, float rotation, const PointF &anchorPos,
        const RGBA &rgba, int filtering) override;

    int GetTexWidth() override { return myTexWidth; }
    int GetTexHeight() override { return myTexHeight; }
    bool GetHasAlpha() override { return myHasAlpha; }

private:
    IDirect3DTexture9* myTexture = nullptr;
    int myWidth = 0;
    int myHeight = 0;
    int myTexWidth = 0;
    int myTexHeight = 0;
    bool myHasAlpha = false;
};

#endif // WINDOWS_VERSION

#endif // SPRITE3D_D3D9RENDEROBJECT_H
