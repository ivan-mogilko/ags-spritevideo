#ifndef SPRITEVIDEO_OGLRENDEROBJECT_H
#define SPRITEVIDEO_OGLRENDEROBJECT_H

#include "RenderObject.h"
#include "MathHelper.h"
#include "OGLHelper.h"

class OGLRenderObject : public RenderObject
{
public:
    OGLRenderObject() = default;
    ~OGLRenderObject() override;

    void CreateTexture(int sprite_id, int bkg_num, const char *file) override;
    void CreateTexture(const unsigned char* data, int width, int height, int bpp) override;
    void Render(const Point &pos, const PointF &scaling, float rotation, const PointF &anchorPos,
        const RGBA &rgba, int filtering) override;

    int GetTexWidth() override { return myTexWidth; }
    int GetTexHeight() override { return myTexHeight; }
    bool GetHasAlpha() override { return myHasAlpha; }

    static bool CreateStaticData();

private:
    unsigned myTexture = 0u;
    int myWidth = 0;
    int myHeight = 0;
    int myTexWidth = 0;
    int myTexHeight = 0;
    bool myHasAlpha = false;

    static OGLCUSTOMVERTEX defaultVertices[4];
    static ShaderProgram defaultProgram;
};

#endif // SPRITEVIDEO_OGLRENDEROBJECT_H
