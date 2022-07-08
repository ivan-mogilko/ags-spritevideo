#include <list>
#include <memory>
#include <string>
#include "Common.h"
#include "BaseObject.h"

// Sprite3D plugin:
// v0.9:
// - OpenGL render support
// - Linux build support

// AJA's Direct3D plugin:
// v1.1:
// - Added D3D.OpenBackground( int frame )
// - Tinting and transparency
// - Limited parenting: position, rotation, scaling, tint, alpha. Rotating a child sprite doesn't work as local rotation, it adds to the global rotation.
// - Fixed float values

// ****** RUN TIME PLUGIN ********

#if defined (WINDOWS_VERSION)
#include "d3d9/D3D9Factory.h"
#endif
#include "ogl/OGLFactory.h"


IAGSEngine* engine = nullptr;
Screen screen;
std::unique_ptr<RenderFactory> factory;

extern void RegisterScriptAPI();

IAGSEngine* GetAGS()
{
    return engine;
}

Screen const* GetScreen()
{
    return &screen;
}

RenderFactory* CreateFactory(const char* driverid)
{
#if defined (WINDOWS_VERSION)
    if (stricmp(driverid, "d3d9") == 0)
    {
        factory = std::make_unique<D3D9Factory>();
    }
    else
#endif // WINDOWS_VERSION
    if (stricmp(driverid, "ogl") == 0)
    {
        factory = std::make_unique<OGLFactory>();
    }
    return factory.get();
}

RenderFactory* GetFactory()
{
    return factory.get();
}

std::list< BaseObject* > manualRenderBatch;

int AGS_PluginV2()
{
    return 1;
}

void AGS_EngineStartup( IAGSEngine *lpEngine )
{
    engine = lpEngine;

    OPEN_DBG( "debug.txt" );
    DBG( "Register" );

    // Make sure it's got the version with the features we need
    DBGF("Engine interface: %d", engine->version);
    if (engine->version < 23)
    {
        DBG( "Abort" );
        engine->AbortGame( "Engine interface is too old, need version of AGS with interface version 25 or higher." );
        return;
    }

    engine->RequestEventHook( AGSE_SAVEGAME );
    engine->RequestEventHook( AGSE_RESTOREGAME );
    engine->RequestEventHook( AGSE_PRERENDER );
    engine->RequestEventHook( AGSE_PRESCREENDRAW );
    engine->RequestEventHook( AGSE_PREGUIDRAW );
    engine->RequestEventHook( AGSE_POSTSCREENDRAW );
    engine->RequestEventHook( AGSE_FINALSCREENDRAW );

    RegisterScriptAPI();
    
    DBG( "Startup" );
}

void AGS_EngineInitGfx( char const* driverID, void* data )
{
    if (!CreateFactory(driverID))
    {
        std::string msg = "Unable to initialize plugin: graphics renderer not supported (";
        msg += driverID; msg += ").";
        engine->AbortGame(msg.c_str());
        return;
    }

    GetFactory()->InitGfxMode(&screen, data);
    //DBGF( "Running at %dx%dx%d", screen.backBufferWidth, screen.backBufferHeight, screen.bpp );
}

void AGS_EngineShutdown()
{
    // Dispose any resources and objects
    DBG( "Shutting down" );

    factory.reset();

    CLOSE_DBG();
}

void Save( int handle )
{
    // Screen
    DBGF( "SAVE frameDelay: %f", screen.frameDelay );
    engine->FWrite( &screen.frameDelay, sizeof( screen.frameDelay ), handle );
    DBGF( "SAVE gameSpeed: %d", screen.gameSpeed );
    engine->FWrite( &screen.gameSpeed, sizeof( screen.gameSpeed ), handle );
}

void Restore( int handle )
{
    // Screen
    engine->FRead( &screen.frameDelay, sizeof( screen.frameDelay ), handle );
    DBGF( "RESTORE frameDelay: %f", screen.frameDelay );
    engine->FRead( &screen.gameSpeed, sizeof( screen.gameSpeed ), handle );
    DBGF( "RESTORE gameSpeed: %d", screen.gameSpeed );
}

void Render( BaseObject::RenderStage stage )
{
    engine->GetScreenDimensions(&screen.width, &screen.height, &screen.bpp);
    DBGF( "RENDER screen %dx%d", screen.width, screen.height );
    // Engine interface >= 25 provides transform matrixes
    if (engine->version >= 25)
    {
        AGSRenderStageDesc desc = {0};
        desc.Version = 25;
        engine->GetRenderStageDesc(&desc);
        GetFactory()->SetScreenMatrixes(&screen, &desc.Matrixes.WorldMatrix, &desc.Matrixes.ViewMatrix, &desc.Matrixes.ProjMatrix);
    }
    else
    {
        GetFactory()->SetScreenMatrixes(&screen, nullptr, nullptr, nullptr);
    }
	
    BaseObject::RenderAll( stage );

    for ( auto i = manualRenderBatch.begin(); i != manualRenderBatch.end(); ++i )
    {
        if ( (*i)->GetRenderStage() == stage )
        {
            (*i)->Render();
        }
    }
}

int AGS_EngineOnEvent( int ev, int data )
{
    if ( ev == AGSE_SAVEGAME )
    {
        Save( data );
    }
    else if ( ev == AGSE_RESTOREGAME )
    {
        Restore( data );
    }
    else if ( ev == AGSE_PRERENDER )
    {
        BaseObject::UpdateAll();
    }
    else if ( ev == AGSE_PRESCREENDRAW )
    {
        // Support older versions, where some renderers may have forgotten
        // to call the "init gfx mode" event
        if (!GetFactory() && !CreateFactory(GetAGS()->GetGraphicsDriverID()))
        {
            std::string msg = "Unable to initialize plugin: graphics renderer not supported (";
            msg += GetAGS()->GetGraphicsDriverID(); msg += ").";
            engine->AbortGame(msg.c_str());
            return 0;
        }

        // Save viewport
        screen.viewport.x = 0;
        screen.viewport.y = 0;
        if (engine->version < 25)
        { // With API 25+ we query transformation matrixes that already tell us this (and more)
            engine->RoomToViewport( &screen.viewport.x, &screen.viewport.y );
        }

        // FIXME: won't work on 64-bit systems!!! use extended engine API?
        GetFactory()->InitGfxDevice(reinterpret_cast<void*>(data));

        Render( BaseObject::STAGE_BACKGROUND );
    }
    else if ( ev == AGSE_PREGUIDRAW )
    {
        Render( BaseObject::STAGE_SCENE );
    }
    else if ( ev == AGSE_POSTSCREENDRAW )
    {
        Render( BaseObject::STAGE_GUI );
    }
    else if ( ev == AGSE_FINALSCREENDRAW )
    {
        Render( BaseObject::STAGE_SCREEN );

        // Clear batch
        manualRenderBatch.clear();
    }

    return 0;
}

// *** END RUN TIME ****

#if defined (WINDOWS_VERSION)

// DllMain - standard Windows DLL entry point.
// The AGS editor will cause this to get called when the editor first
// starts up, and when it shuts down at the end.
BOOL APIENTRY DllMain(HANDLE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }

    return TRUE;
}

#endif // WINDOWS_VERSION
