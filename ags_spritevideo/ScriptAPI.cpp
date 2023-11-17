#include <inttypes.h>
#include <list>
#include "Common.h"
#include "SpriteObject.h"
#include "StringHelper.h"
#include "VideoObject.h"

// AGS:n float-tyypin muunnokset
#define SCRIPT_FLOAT(x) int32_t __script_float##x
#define INIT_SCRIPT_FLOAT(x) float x = *((float*)&__script_float##x)
#define FLOAT_RETURN_TYPE int32_t
#define RETURN_FLOAT(x) return *((int32_t*)&x)


extern Screen screen;
extern std::list< BaseObject* > manualRenderBatch;

// *** D3D ***
#if defined (VIDEO_PLAYBACK)
VideoObject_Manager videoObjManager;
#endif
SpriteObject_Manager spriteObjManager;

void D3D_SetGameSpeed(int speed)
{
    screen.gameSpeed = speed;
    screen.frameDelay = 1.f / speed;
}

SpriteObject* D3D_OpenSprite(int spriteID)
{
    SpriteObject* obj = SpriteObject::Open(spriteID);

    if (obj)
    {
        GetAGS()->RegisterManagedObject(obj, &spriteObjManager);
    }

    return obj;
}

SpriteObject* D3D_OpenSpriteFile(char const* filename, int filtering)
{
    SpriteObject* obj = SpriteObject::Open(filename, (BaseObject::Filtering)filtering);

    if (obj)
    {
        GetAGS()->RegisterManagedObject(obj, &spriteObjManager);
    }

    return obj;
}

SpriteObject* D3D_OpenBackground(int frame)
{
    SpriteObject* obj = SpriteObject::OpenBackground(frame);

    if (obj)
    {
        GetAGS()->RegisterManagedObject(obj, &spriteObjManager);
    }

    return obj;
}

#if defined (VIDEO_PLAYBACK)
VideoObject* D3D_OpenVideo(char const* filename)
{
    VideoObject* obj = VideoObject::Open(filename, GetFactory()->IsRGBA());

    if (obj)
    {
        GetAGS()->RegisterManagedObject(obj, &videoObjManager);
    }

    return obj;
}
#endif


// *** BaseObject ***
void D3DObject_SetEnabled(BaseObject* obj, bool enabled) { obj->SetEnabled(enabled); }
int D3DObject_GetEnabled(BaseObject* obj) { return obj->IsEnabled(); }
void D3DObject_SetVisible(BaseObject* obj, bool visible) { obj->SetVisible(visible); }
int D3DObject_GetVisible(BaseObject* obj) { return obj->IsVisible(); }
void D3DObject_SetX(BaseObject* obj, int x) { int y = obj->GetPosition().y; obj->SetPosition(Point(x, y)); }
int D3DObject_GetX(BaseObject* obj) { return obj->GetPosition().x; }
void D3DObject_SetY(BaseObject* obj, int y) { int x = obj->GetPosition().x; obj->SetPosition(Point(x, y)); }
int D3DObject_GetY(BaseObject* obj) { return obj->GetPosition().y; }
int D3DObject_GetWidth(BaseObject* obj) { return obj->GetWidth(); }
int D3DObject_GetHeight(BaseObject* obj) { return obj->GetHeight(); }

void D3DObject_SetAnchorX(BaseObject* obj, SCRIPT_FLOAT(x)) {
    INIT_SCRIPT_FLOAT(x);
    float y = obj->GetAnchor().y; obj->SetAnchor(PointF(x, y));
}
FLOAT_RETURN_TYPE D3DObject_GetAnchorX(BaseObject* obj) {
    float x = obj->GetAnchor().x;
    RETURN_FLOAT(x);
}

void D3DObject_SetAnchorY(BaseObject* obj, SCRIPT_FLOAT(y)) {
    INIT_SCRIPT_FLOAT(y);
    float x = obj->GetAnchor().x; obj->SetAnchor(PointF(x, y));
}
FLOAT_RETURN_TYPE D3DObject_GetAnchorY(BaseObject* obj) {
    float y = obj->GetAnchor().y;
    RETURN_FLOAT(y);
}

void D3DObject_SetRotation(BaseObject* obj, SCRIPT_FLOAT(rot)) {
    INIT_SCRIPT_FLOAT(rot);
    obj->SetRotation(rot);
}
FLOAT_RETURN_TYPE D3DObject_GetRotation(BaseObject* obj) {
    float r = obj->GetRotation();
    RETURN_FLOAT(r);
}

void D3DObject_SetScaling(BaseObject* obj, SCRIPT_FLOAT(scaling)) {
    INIT_SCRIPT_FLOAT(scaling);
    obj->SetScaling(scaling);
}
FLOAT_RETURN_TYPE D3DObject_GetScaling(BaseObject* obj) {
    float s = obj->GetScaling().x;
    RETURN_FLOAT(s);
}

void D3DObject_SetTintR(BaseObject* obj, SCRIPT_FLOAT(r)) {
    INIT_SCRIPT_FLOAT(r);
    obj->SetTintR(r);
}
FLOAT_RETURN_TYPE D3DObject_GetTintR(BaseObject* obj) {
    float r = obj->GetTintR();
    RETURN_FLOAT(r);
}

void D3DObject_SetTintG(BaseObject* obj, SCRIPT_FLOAT(r)) {
    INIT_SCRIPT_FLOAT(r);
    obj->SetTintG(r);
}
FLOAT_RETURN_TYPE D3DObject_GetTintG(BaseObject* obj) {
    float g = obj->GetTintG();
    RETURN_FLOAT(g);
}

void D3DObject_SetTintB(BaseObject* obj, SCRIPT_FLOAT(r)) {
    INIT_SCRIPT_FLOAT(r);
    obj->SetTintB(r);
}
FLOAT_RETURN_TYPE D3DObject_GetTintB(BaseObject* obj) {
    float b = obj->GetTintB();
    RETURN_FLOAT(b);
}

void D3DObject_SetAlpha(BaseObject* obj, SCRIPT_FLOAT(a)) {
    INIT_SCRIPT_FLOAT(a); obj->SetAlpha(a);
}
FLOAT_RETURN_TYPE D3DObject_GetAlpha(BaseObject* obj) {
    float a = obj->GetAlpha();
    RETURN_FLOAT(a);
}

void D3DObject_SetAutoUpdated(BaseObject* obj, bool autoUpdated) { obj->SetAutoUpdated(autoUpdated); }
int D3DObject_GetAutoUpdated(BaseObject* obj) { return obj->IsAutoUpdated(); }
void D3DObject_SetAutoRendered(BaseObject* obj, bool autoRendered) { obj->SetAutoRendered(autoRendered); }
int D3DObject_GetAutoRendered(BaseObject* obj) { return obj->IsAutoRendered(); }
void D3DObject_SetRenderStage(BaseObject* obj, int stage) { obj->SetRenderStage((BaseObject::RenderStage)stage); }
int D3DObject_GetRenderStage(BaseObject* obj) { return (int)obj->GetRenderStage(); }
void D3DObject_SetRelativeTo(BaseObject* obj, int relative) { obj->SetRelativeTo((BaseObject::RelativeTo)relative); }
int D3DObject_GetRelativeTo(BaseObject* obj) { return (int)obj->GetRelativeTo(); }
void D3DObject_SetRoom(BaseObject* obj, int room) { obj->SetRoom(room); }
int D3DObject_GetRoom(BaseObject* obj) { return obj->GetRoom(); }
void D3DObject_SetPosition(BaseObject* obj, int x, int y) { obj->SetPosition(Point(x, y)); }

void D3DObject_SetAnchor(BaseObject* obj, SCRIPT_FLOAT(x), SCRIPT_FLOAT(y)) {
    INIT_SCRIPT_FLOAT(x);
    INIT_SCRIPT_FLOAT(y);
    obj->SetAnchor(PointF(x, y));
}

void D3DObject_SetTint(BaseObject* obj, SCRIPT_FLOAT(r), SCRIPT_FLOAT(g), SCRIPT_FLOAT(b)) {
    INIT_SCRIPT_FLOAT(r);
    INIT_SCRIPT_FLOAT(g);
    INIT_SCRIPT_FLOAT(b);
    obj->SetTint(r, g, b);
}

void D3DObject_SetParent(BaseObject* obj, int key) { obj->SetParent((BaseObject*)GetAGS()->GetManagedObjectAddressByKey(key)); }
int D3DObject_GetKey(BaseObject* obj) { return GetAGS()->GetManagedObjectKeyByAddress((char*)obj); }
void D3DObject_Update(BaseObject* obj) { obj->Update(); }
void D3DObject_Render(BaseObject* obj) { manualRenderBatch.push_back(obj); }

// *** VideoObject ***
#if defined (VIDEO_PLAYBACK)

void D3DVideoObject_SetLooping(VideoObject* obj, bool loop) { obj->SetLooping(loop); }
int D3DVideoObject_GetLooping(VideoObject* obj) { return obj->IsLooping(); }

void D3DVideoObject_SetFPS(VideoObject* obj, SCRIPT_FLOAT(fps)) {
    INIT_SCRIPT_FLOAT(fps);
    obj->SetFPS(fps);
}
FLOAT_RETURN_TYPE D3DVideoObject_GetFPS(VideoObject* obj) {
    float fps = obj->GetFPS();
    RETURN_FLOAT(fps);
}

int D3DVideoObject_NextFrame(VideoObject* obj) { return obj->NextFrame(); }
void D3DVideoObject_Autoplay(VideoObject* obj) { obj->Autoplay(); }
int D3DVideoObject_IsAutoplaying(VideoObject* obj) { return obj->IsAutoplaying(); }
void D3DVideoObject_StopAutoplay(VideoObject* obj) { obj->StopAutoplay(); }

#endif


void dummy(BaseObject* obj) {}

#define REG( name, func ) { engine->RegisterScriptFunction( name, reinterpret_cast<void*>(func) ); }

#define REG_D3DOBJECT_BASE( cname ) \
    REG( cname "::set_isEnabled", D3DObject_SetEnabled );\
    REG( cname "::get_isEnabled", D3DObject_GetEnabled );\
    REG( cname "::set_isVisible", D3DObject_SetVisible );\
    REG( cname "::get_isVisible", D3DObject_GetVisible );\
    REG( cname "::set_x", D3DObject_SetX );\
    REG( cname "::get_x", D3DObject_GetX );\
    REG( cname "::set_y", D3DObject_SetY );\
    REG( cname "::get_y", D3DObject_GetY );\
    REG( cname "::get_width", D3DObject_GetWidth );\
    REG( cname "::set_width", dummy );\
    REG( cname "::get_height", D3DObject_GetHeight );\
    REG( cname "::set_height", dummy );\
    REG( cname "::set_anchorX", D3DObject_SetAnchorX );\
    REG( cname "::get_anchorX", D3DObject_GetAnchorX );\
    REG( cname "::set_anchorY", D3DObject_SetAnchorY );\
    REG( cname "::get_anchorY", D3DObject_GetAnchorY );\
    REG( cname "::set_rotation", D3DObject_SetRotation );\
    REG( cname "::get_rotation", D3DObject_GetRotation );\
    REG( cname "::set_scaling", D3DObject_SetScaling );\
    REG( cname "::get_scaling", D3DObject_GetScaling );\
	REG( cname "::set_tintR", D3DObject_SetTintR );\
	REG( cname "::get_tintR", D3DObject_GetTintR );\
	REG( cname "::set_tintG", D3DObject_SetTintG );\
	REG( cname "::get_tintG", D3DObject_GetTintG );\
	REG( cname "::set_tintB", D3DObject_SetTintB );\
	REG( cname "::get_tintB", D3DObject_GetTintB );\
	REG( cname "::set_alpha", D3DObject_SetAlpha );\
	REG( cname "::get_alpha", D3DObject_GetAlpha );\
    REG( cname "::set_isAutoUpdated", D3DObject_SetAutoUpdated );\
    REG( cname "::get_isAutoUpdated", D3DObject_GetAutoUpdated );\
    REG( cname "::set_isAutoRendered", D3DObject_SetAutoRendered );\
    REG( cname "::get_isAutoRendered", D3DObject_GetAutoRendered );\
    REG( cname "::set_renderStage", D3DObject_SetRenderStage );\
    REG( cname "::get_renderStage", D3DObject_GetRenderStage );\
    REG( cname "::set_relativeTo", D3DObject_SetRelativeTo );\
    REG( cname "::get_relativeTo", D3DObject_GetRelativeTo );\
    REG( cname "::set_room", D3DObject_SetRoom );\
    REG( cname "::get_room", D3DObject_GetRoom );\
    REG( cname "::SetPosition^2", D3DObject_SetPosition );\
    REG( cname "::SetAnchor^2", D3DObject_SetAnchor );\
	REG( cname "::SetTint^3", D3DObject_SetTint );\
	REG( cname "::SetParent^1", D3DObject_SetParent );\
	REG( cname "::GetKey^0", D3DObject_GetKey );\
    REG( cname "::Update^0", D3DObject_Update );\
    REG( cname "::Render^0", D3DObject_Render )


void testCall()
{
    IAGSEngine* engine = GetAGS();

    void(*func)(const char*, ...) = (void(*)(const char*, ...))engine->GetScriptFunctionAddress("Display");

    DBGF("%" PRIxPTR "", (intptr_t)engine->GetScriptFunctionAddress("Character::Say^0"));
    func("%" PRIxPTR "", (intptr_t)engine->GetScriptFunctionAddress("Character::Say^3"));

    DBGF("%" PRIxPTR "", (intptr_t)engine->GetScriptFunctionAddress("Character::LockView^1"));
    func("%" PRIxPTR "", (intptr_t)engine->GetScriptFunctionAddress("Character::LockView^1"));
}


void RegisterScriptAPI()
{
    IAGSEngine* engine = GetAGS();

    // Lukijat
    engine->AddManagedObjectReader(spriteObjManager.GetType(), &spriteObjManager);

    // D3D
    REG("D3D::SetLoopsPerSecond", D3D_SetGameSpeed);
    REG("D3D::OpenSprite", D3D_OpenSprite);
    REG("D3D::OpenSpriteFile", D3D_OpenSpriteFile);
    REG("D3D::OpenBackground", D3D_OpenBackground);

    // D3DSprite
    REG_D3DOBJECT_BASE("D3D_Sprite");

#if defined (VIDEO_PLAYBACK)
    // D3D
    REG("D3D::OpenVideo", D3D_OpenVideo);

    // D3DVideo
    REG_D3DOBJECT_BASE("D3D_Video");
    REG("D3D_Video::set_isLooping", D3DVideoObject_SetLooping);
    REG("D3D_Video::get_isLooping", D3DVideoObject_GetLooping);
    REG("D3D_Video::set_fps", D3DVideoObject_SetFPS);
    REG("D3D_Video::get_fps", D3DVideoObject_GetFPS);

    REG("D3D_Video::NextFrame^0", D3DVideoObject_NextFrame);
    REG("D3D_Video::Autoplay^0", D3DVideoObject_Autoplay);
    REG("D3D_Video::IsAutoplaying^0", D3DVideoObject_IsAutoplaying);
    REG("D3D_Video::StopAutoplay^0", D3DVideoObject_StopAutoplay);
#endif // VIDEO_PLAYBACK

    REG("testCall", testCall);
}
