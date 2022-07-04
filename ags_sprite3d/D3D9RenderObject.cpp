#include "D3D9RenderObject.h"
#include <d3dx9.h>
#include "Common.h"
#include "D3D9Global.h"
#include "D3DHelper.h"
#include "BaseObject.h" // FIXME


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
            DBG("Could not open sprite #%d", sprite_id);
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
            DBG("Could not open room background #%d", bkg_num);
        }
    }
    else if (file)
    {
        D3DXIMAGE_INFO info;

        // TODO: Tutki paddingia POT:ksi http://subversion.assembla.com/svn/AvP/trunk/xbox/src/d3_func.cpp
        if (FAILED(D3DXCreateTextureFromFileExA(GetD3D(),
            file,
            D3DX_DEFAULT_NONPOW2,
            D3DX_DEFAULT_NONPOW2,
            1,
            0,
            D3DFMT_UNKNOWN,
            D3DPOOL_MANAGED,
            D3DX_DEFAULT,
            D3DX_DEFAULT,
            0,
            &info,
            NULL,
            &myTexture)))
        {
            DBG("Texture creation failed");
        }
        else
        {
            DBG("OK");

            // Image size and alpha
            myWidth = info.Width;
            myHeight = info.Height;
            myHasAlpha = info.Format == D3DFMT_A8R8G8B8;

            // Texture size
            D3DSURFACE_DESC desc;
            myTexture->GetLevelDesc(0, &desc);
            myTexWidth = desc.Width;
            myTexHeight = desc.Height;
        }
    }
}

void D3D9RenderObject::Render(const Point &pos, const PointF &scaling, float rotation,
    const PointF &anchorPos, const RGBA &rgba, int filter)
{
    DBG("myRender::Render");
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

    float screenScaleX = 1.0;//static_cast<float>(screen->backBufferWidth) / screen->width;
    float screenScaleY = 1.0;//static_cast<float>(screen->backBufferHeight) / screen->height;
    if (screen->matrixValid)
    {
        float orthoSizeX = 2.f / screen->globalProj._11;
        float orthoSizeY = 2.f / screen->globalProj._22;
        screenScaleX = orthoSizeX / screen->width;
        screenScaleY = orthoSizeY / screen->height;
        DBG("---ORTHO w = %f, h = %f", orthoSizeX, orthoSizeY);
    }
    else
    {
    }

    DBG("---RENDER screenScale: %f,%f", screenScaleX, screenScaleY);

    // World matrix, set position, anchor, rotation and scaling
    D3DMATRIX trans, scale, rot, anchor;
    DBG("---RENDER TRANS: %f,%f", pos.x - screenScaleX * GetScreen()->width / 2.f, pos.y - (1.f + 1.f - screenScaleY) * GetScreen()->height / 2.f);
    SetMatrix(&trans, pos.x - screenScaleX * GetScreen()->width / 2.f,
        pos.y - (1.f + 1.f - screenScaleY) * GetScreen()->height / 2.f,
        1, 1);
    DBG("---RENDER SCALE: %f,%f", screenScaleX * myWidth * scaling.x, screenScaleY * myHeight * scaling.y);
    SetMatrix(&scale, 0, 0, screenScaleX * myWidth * scaling.x, screenScaleY * myHeight * scaling.y);
    DBG("---RENDER ROTATION: %f at %f,%f", rotation, -anchorPos.x, anchorPos.y);
    SetMatrix(&anchor, -anchorPos.x, anchorPos.y, 1, 1); // Mirror Y
    SetMatrixRotation(&rot, rotation * RADS_PER_DEGREE);

    D3DMATRIX world;
    // Apply self-transform first
    MatrixMultiply(&world, &anchor, &scale);
    MatrixMultiply(&world, &world, &rot);
    MatrixMultiply(&world, &world, &trans);
    // Apply global world matrix too
    MatrixMultiply(&world, &world, reinterpret_cast<const D3DMATRIX*>(&screen->globalWorld));

    // Set transforms
    device->SetTransform(D3DTS_WORLD, &world);
    device->SetTransform(D3DTS_VIEW, reinterpret_cast<const D3DMATRIX*>(&screen->globalView));

    // Scale texture coordinates
    float scaleU = myTexWidth / static_cast<float>(myWidth);
    float scaleV = myTexHeight / static_cast<float>(myHeight);

    D3DMATRIX tex;
    SetMatrix(&tex, .5f, -150, 1, .5f);
    device->SetTransform(D3DTS_TEXTURE1, &tex);


    // Store old vertex format
    DWORD oldFVF;
    device->GetFVF(&oldFVF);

    if (filter == BaseObject::FILTER_LINEAR) // FIXME declare elsewhere
    {
        // Linear texture filtering
        device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    }
    else if (filter == BaseObject::FILTER_NEAREST) // FIXME declare elsewhere
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
        DBG("Draw failed");
    }

    // Restore old vertex format
    device->SetFVF(oldFVF);
}
