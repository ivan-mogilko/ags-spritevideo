#ifndef SPRITEVIDEO_OGLFACTORY_H
#define SPRITEVIDEO_OGLFACTORY_H

#include "RenderFactory.h"

class OGLFactory : public RenderFactory
{
public:
    void InitGfxDevice(void* data) override;
    bool InitGfxMode(Screen* screen, void* data) override;
    void SetScreenMatrixes(Screen* screen, float(*world)[16], float(*view)[16], float(*proj)[16]) override;
    std::unique_ptr<RenderObject> CreateRenderObject() override;
    bool IsRGBA() override;
};

#endif // SPRITEVIDEO_OGLFACTORY_H
