/**
 * @file Common.h
 * @author Ahonen
 *
 * @brief Common stuff. Includes, debug, etc.
 */

#ifndef SPRITE3D_COMMON_H
#define SPRITE3D_COMMON_H

#if defined (WINDOWS_VERSION)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif
#include <windows.h>
#endif
// Plugin API
#define THIS_IS_THE_PLUGIN
#include "agsplugin.h"

// Disabled warnings
#pragma warning(disable : 4251) // dll string warnings
#pragma warning(disable : 4996) // sprintf_s etc.

// Debug printing
#define DEBUG

// TODO: move to a separate helper h/cpp
#ifndef MAX_PATH
#define MAX_PATH (512)
#endif

#if !defined (WINDOWS_VERSION)
inline stricmp(const char* a, const char* b) { return strcasecmp(a, b); }
#endif

#ifdef DEBUG
    #include <cstdio>    

    extern FILE* debug;
    #define DBG( x ) { fprintf( debug, "%s\n", x ); fflush( debug ); }
    #define DBGF( x, ... ) { fprintf( debug, x"\n", __VA_ARGS__ ); fflush( debug ); }
    #define OPEN_DBG( filename ) { debug = fopen( filename, "w" ); }
    #define CLOSE_DBG() { fclose( debug ); debug = NULL; }
#else
    #define DBG( x, ... )
    #define DBGF( x, ... )
    #define OPEN_DBG( filename )
    #define CLOSE_DBG()
#endif

// Use only within Serialize methods!
#define SERIALIZE( var )\
    {\
        DBGF( "SERIALIZE( " #var " ): %f", static_cast<float>( var ) );\
        buffer = (char*)memcpy( buffer, &var, sizeof( var ) ) + sizeof( var );\
    }
#define SERIALIZE_STR( str )\
    {\
        if ( str.empty() ) { size_t n = 0; SERIALIZE( n ); DBGF( "SERIALIZE_STR( NULL )"); }\
        else {\
            size_t len = str.size();\
            SERIALIZE( len );\
            DBGF( "SERIALIZE_STR( %s )", str.c_str() );\
            buffer = (char*)memcpy( buffer, str.c_str(), len ) + len;\
        }\
    }

// Use only within Unserialize methods!
#define UNSERIALIZE( var )\
    {\
        memcpy( &var, buffer, sizeof( var ) ); buffer += sizeof( var );\
        DBGF( "UNSERIALIZE( " #var " ): %f", static_cast<float>( var ) );\
    }
#define UNSERIALIZE_STR( str )\
    {\
        size_t n; UNSERIALIZE( n );\
        if ( n == 0 ) { str.clear(); DBG( "UNSERIALIZE_STR( NULL )" ); }\
        else {\
            str.resize(n);\
            memcpy( &str[0], buffer, n );\
            DBGF( "UNSERIALIZE_STR( %s )", str.c_str() );\
        }\
        buffer += n;\
    }


#include <cstring>
#include "MathHelper.h"
#include "RenderFactory.h"

struct Screen
{
    int width = 0;
    int height = 0;
    int bpp = 0;
    Point viewport;
    int gameSpeed = 40;
    float frameDelay = 1.f / 40;

    // Render stage transform matrixes
    bool matrixValid = false;
    Matrix globalWorld;
    Matrix globalView;
    Matrix globalProj;

    Screen()
    {
        memset(&globalWorld, 0, sizeof(float[16]));
        memset(&globalView, 0, sizeof(float[16]));
        memset(&globalProj, 0, sizeof(float[16]));
    }

    Point FromRoom(Point pt) const
    {
        return Point(pt.x + viewport.x, height - pt.y - viewport.y);
    }

    Point FromScreen(Point pt) const
    {
        return Point(pt.x, height - pt.y);
    }
};

// *** Global functions ***
IAGSEngine* GetAGS();
Screen const* GetScreen();
RenderFactory* CreateFactory(const char* driverid);
RenderFactory* GetFactory();

#endif // SPRITE3D_COMMON_H
