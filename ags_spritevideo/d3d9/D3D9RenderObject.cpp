#if defined (WINDOWS_VERSION)

#include "D3D9RenderObject.h"
#include <vector>
#include <d3dx9.h>
#include "Common.h"
#include "BaseObject.h"
#include "D3DHelper.h"
#include "D3D9Factory.h"
#include "ImageHelper.h"


D3D9RenderObject::~D3D9RenderObject()
{
    if (myTexture)
    {
        myTexture->Release();
        myTexture = NULL;
    }
}

void D3D9RenderObject::CreateTexture(int sprite_id, int bkg_num, const char *file)
{
    if (sprite_id >= 0)
    {
        BITMAP* bmp = GetAGS()->GetSpriteGraphic(sprite_id);
        myWidth = GetAGS()->GetSpriteWidth(sprite_id);
        myHeight = GetAGS()->GetSpriteHeight(sprite_id);
        myHasAlpha = GetAGS()->IsSpriteAlphaBlended(sprite_id) != 0;
        myTexWidth = myWidth;
        myTexHeight = myHeight;

        unsigned char** data = GetAGS()->GetRawBitmapSurface(bmp);

        myTexture = ::CreateTexture(data, myWidth, myHeight, myHasAlpha);

        if (!myTexture)
        {
            DBGF("Could not open sprite #%d", sprite_id);
        }
    }
    else if (bkg_num >= 0)
    {
        BITMAP* bmp = GetAGS()->GetBackgroundScene(bkg_num);
        int unused;
        GetAGS()->GetBitmapDimensions(bmp, &myWidth, &myHeight, &unused);
        myHasAlpha = false;
        myTexWidth = myWidth;
        myTexHeight = myHeight;

        unsigned char** data = GetAGS()->GetRawBitmapSurface(bmp);

        myTexture = ::CreateTexture(data, myWidth, myHeight);

        if (!myTexture)
        {
            DBGF("Could not open room background #%d", bkg_num);
        }
    }
    else if (file)
    {
        ImageInfo info;
        std::vector<unsigned char> data;
        if (LoadImage(file, data, info))
        {
            myWidth = info.Width;
            myHeight = info.Height;
            myTexWidth = myWidth;
            myTexHeight = myHeight;
            myHasAlpha = info.HasAlpha;
            myTexture = ::CreateTexture(&data[0], info.Width, info.Height, info.HasAlpha);
        }

        if (!myTexture)
        {
            DBGF("Could not create texture from file %s", file);
        }
    }
}

void D3D9RenderObject::CreateTexture(const unsigned char* data, int width, int height, int bpp)
{
    if (!myTexture || myTexWidth != width || myTexHeight != height)
    {
        if (myTexture)
            myTexture->Release();

        if (width <= 0 || height <= 0 || bpp <= 0)
            return; // fail

        myTexture = ::CreateTexture(data, width, height);
        myWidth = width;
        myHeight = height;
        myTexWidth = width;
        myTexHeight = height;
        myHasAlpha = false; // CHECKME??
    }
    else
    {
        if (width <= 0 || height <= 0 || bpp <= 0)
            return; // fail

        ::SetTextureData(myTexture, data, width, height);
    }
}

void D3D9RenderObject::Render(const Point &pos, const PointF &scaling, float rotation,
    const PointF &anchorPos, const RGBA &rgba, int filter)
{
    //DBG("myRender::Render");
    IDirect3DDevice9* device = GetD3D();
    const Screen *screen = GetScreen();

    device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

    device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

    device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_SELECTARG2);
    device->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    device->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);

    device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    device->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    device->SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

    device->SetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_DISABLE);
    device->SetTextureStageState(2, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

    device->SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);

    float screenScaleX = 1.0;
    float screenScaleY = 1.0;
    if (screen->matrixValid)
    {
        float orthoSizeX = 2.f / screen->globalProj._11;
        float orthoSizeY = 2.f / screen->globalProj._22;
        screenScaleX = orthoSizeX / screen->width;
        screenScaleY = orthoSizeY / screen->height;
        //DBGF("---ORTHO w = %f, h = %f", orthoSizeX, orthoSizeY);
    }
    else
    {
    }

    //DBGF("---RENDER screenScale: %f,%f", screenScaleX, screenScaleY);

    // World matrix, set position, anchor, rotation and scaling
    Matrix trans, scale, rot, anchor;
    //DBGF("---RENDER TRANS: %f,%f", pos.x - screenScaleX * screen->width / 2.f, pos.y - (1.f + 1.f - screenScaleY) * screen->height / 2.f);
    SetMatrix(&trans, pos.x - screenScaleX * screen->width / 2.f,
        pos.y - (1.f + 1.f - screenScaleY) * screen->height / 2.f,
        1, 1);
    //DBGF("---RENDER SCALE: %f,%f", screenScaleX * myWidth * scaling.x, screenScaleY * myHeight * scaling.y);
    SetMatrix(&scale, 0, 0, screenScaleX * myWidth * scaling.x, screenScaleY * myHeight * scaling.y);
    //DBGF("---RENDER ROTATION: %f at %f,%f", rotation, -anchorPos.x, anchorPos.y);
    SetMatrix(&anchor, -anchorPos.x, anchorPos.y, 1, 1); // Mirror Y
    SetMatrixRotation(&rot, rotation * RADS_PER_DEGREE);

    Matrix world;
    // Apply self-transform first
    MatrixMulD3D(&world, &anchor, &scale);
    MatrixMulD3D(&world, &world, &rot);
    MatrixMulD3D(&world, &world, &trans);
    // Apply global world matrix too
    MatrixMulD3D(&world, &world, &screen->globalWorld);

    // Set transforms
    device->SetTransform(D3DTS_WORLD, reinterpret_cast<const D3DMATRIX*>(&world));
    device->SetTransform(D3DTS_VIEW, reinterpret_cast<const D3DMATRIX*>(&screen->globalView));

    // Scale texture coordinates
    /* CHECKME?
    float scaleU = myTexWidth / static_cast<float>(myWidth);
    float scaleV = myTexHeight / static_cast<float>(myHeight);
    */

    Matrix tex;
    SetMatrix(&tex, .5f, -150, 1, .5f); // FIXME: wtf??
    device->SetTransform(D3DTS_TEXTURE1, reinterpret_cast<const D3DMATRIX*>(&tex));

    // Store old vertex format
    DWORD oldFVF;
    device->GetFVF(&oldFVF);

    if (filter == BaseObject::FILTER_LINEAR)
    {
        // Linear texture filtering
        device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    }
    else if (filter == BaseObject::FILTER_NEAREST)
    {
        // Nearest neighbor filtering
        device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
        device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    }

    // Use the texture
    device->SetTexture(0, myTexture);

    // Set our vertex format
    device->SetFVF(VERTEX_FVF);

    // Copy of the default quad
    Vertex quad[4];
    memcpy(quad, DEFAULT_QUAD, sizeof(Vertex) * 4);

    // Tint and alpha
    D3DCOLOR color = D3DCOLOR_RGBA(static_cast<int>(rgba.r * 255),
        static_cast<int>(rgba.g * 255),
        static_cast<int>(rgba.b * 255),
        static_cast<int>(rgba.a * 255));
    quad[0].color = quad[1].color = quad[2].color = quad[3].color = color;

    if (FAILED(device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, (void*)quad, sizeof(Vertex))))
    {
        //DBG("Draw failed");
    }

    // Restore old vertex format
    device->SetFVF(oldFVF);
}

#endif // WINDOWS_VERSION
