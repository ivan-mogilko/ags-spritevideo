#ifndef SPRITEVIDEO_RENDEROBJECT_H
#define SPRITEVIDEO_RENDEROBJECT_H

#include "MathHelper.h"

class RenderObject
{
public:
    virtual ~RenderObject() = default;

    virtual void CreateTexture(int sprite_id, int bkg_num, const char *file) = 0;
    virtual void CreateTexture(const unsigned char* data, int width, int height, int bpp) = 0;
    virtual void Render(const Point &pos, const PointF &scaling, float rotation, const PointF &anchorPos,
        const RGBA &rgba, int filtering) = 0;
    virtual int GetTexWidth() = 0;
    virtual int GetTexHeight() = 0;
    virtual bool GetHasAlpha() = 0;
};

#endif // SPRITEVIDEO_RENDEROBJECT_H
