#if defined (WINDOWS_VERSION)

#include "Common.h"

// ***** DESIGN TIME PLUGIN *******

#define IMPORT_D3DOBJECT_BASE \
    "   import attribute bool isEnabled;\r\n"\
    "   import attribute bool isVisible;\r\n"\
    "   import attribute int x;\r\n"\
    "   import attribute int y;\r\n"\
    "   readonly import attribute int width;\r\n"\
    "   readonly import attribute int height;\r\n"\
    "   import attribute float anchorX;\r\n"\
    "   import attribute float anchorY;\r\n"\
    "   import attribute float rotation;\r\n"\
    "   import attribute float scaling;\r\n"\
	"	import attribute float tintR;\r\n"\
	"	import attribute float tintG;\r\n"\
	"	import attribute float tintB;\r\n"\
	"	import attribute float alpha;\r\n"\
    "   import attribute bool isAutoUpdated;\r\n"\
    "   import attribute bool isAutoRendered;\r\n"\
    "   import attribute D3D_RenderStage renderStage;\r\n"\
    "   import attribute D3D_RelativeTo relativeTo;\r\n"\
    "   import attribute int room;\r\n"\
    "   import void SetPosition( int x, int y );\r\n"\
    "   import void SetAnchor( float x, float y );\r\n"\
	"	import void SetTint( float r, float g, float b );\r\n"\
	"	import void SetParent( int parentKey );\r\n"\
	"	import int GetKey();\r\n"\
    "   import void Update();\r\n"\
    "   import void Render();\r\n"

IAGSEditor *editor;
const char *ourScriptHeader =

// *** D3D_Filtering ***
"enum D3D_Filtering\r\n"
"{\r\n"
"   eD3D_FilterNearest = 0,\r\n"
"   eD3D_FilterLinear = 1\r\n"
"};\r\n\r\n"

// *** D3D_RenderStage ***
"enum D3D_RenderStage\r\n"
"{\r\n"
"   eD3D_StageBackground = 0,\r\n"
"   eD3D_StageScene = 1,\r\n"
"   eD3D_StageGUI = 2,\r\n"
"   eD3D_StageScreen = 3,\r\n"
"};\r\n\r\n"

// *** D3D_RelativeTo ***
"enum D3D_RelativeTo\r\n"
"{\r\n"
"   eD3D_RelativeToRoom = 0,\r\n"
"   eD3D_RelativeToScreen = 1\r\n"
"};\r\n\r\n"

// *** D3D_Sprite ***
"managed struct D3D_Sprite\r\n"
"{\r\n"

IMPORT_D3DOBJECT_BASE

// SpriteObject
"};\r\n\r\n"

#if defined (VIDEO_PLAYBACK)
// *** D3D_Video ***
"managed struct D3D_Video\r\n"
"{\r\n"

IMPORT_D3DOBJECT_BASE

// D3DVideoObject    
"   import attribute bool isLooping;\r\n"
"   import attribute float fps;\r\n"

"   import bool NextFrame();\r\n"
"   import void Autoplay();\r\n"
"   import bool IsAutoplaying();\r\n"
"   import void StopAutoplay();\r\n"
"};\r\n\r\n"
#endif // VIDEO_PLAYBACK

// *** D3D ****
"struct D3D\r\n"
"{\r\n"
"   import static void SetLoopsPerSecond( int loops );\r\n"
#if defined (VIDEO_PLAYBACK)
"   import static D3D_Video* OpenVideo( String filename );\r\n"
#endif
"   import static D3D_Sprite* OpenSprite( int graphic );\r\n"
"   import static D3D_Sprite* OpenSpriteFile( String filename, D3D_Filtering filtering );\r\n"
"	import static D3D_Sprite* OpenBackground( int frame );\r\n"
"};\r\n"
"import void testCall();\r\n"
;

LPCSTR AGS_GetPluginName()
{
    // Return the plugin description
    return "Direct3D Plugin";
}

int AGS_EditorStartup(IAGSEditor *lpEditor)
{
    // User has checked the plugin to use it in their game

    // If it's an earlier version than what we need, abort.
    if (lpEditor->version < 1)
        return -1;

    editor = lpEditor;
    editor->RegisterScriptHeader(ourScriptHeader);

    // Return 0 to indicate success
    return 0;
}

void AGS_EditorShutdown()
{
    // User has un-checked the plugin from their game
    editor->UnregisterScriptHeader(ourScriptHeader);
}

void AGS_EditorProperties(HWND parent)
{
    // User has chosen to view the Properties of the plugin
    // We could load up an options dialog or something here instead
    MessageBox(parent,
        L"Sprite3D Plugin © 2022 Ivan Mogilko\n\n"
        L"Based on Direct3D Plugin © 2012 Aki Ahonen\n\n"
        L"See ags_d3d.htm for more information.",
        L"About", MB_OK | MB_ICONINFORMATION);
}

int AGS_EditorSaveGame(char *buffer, int bufsize)
{
    // We don't want to save any persistent data
    return 0;
}

void AGS_EditorLoadGame(char *buffer, int bufsize)
{
    // Nothing to load for this dummy plugin
}

// ******* END DESIGN TIME  *******
#endif // WINDOWS_VERSION
