/**
 * @file Common.h
 * @author Ahonen
 *
 * @brief Common stuff. Includes, debug, etc.
 */

#ifndef SPRITE3D_COMMON_H
#define SPRITE3D_COMMON_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// Plugin API
#include "agsplugin.h"

// Disabled warnings
#pragma warning(disable : 4251) // dll string warnings
#pragma warning(disable : 4996) // sprintf_s etc.

// Constants and math
float const RADS_PER_DEGREE = 3.14159265f / 180.f;

// Debug printing
#define DEBUG

#ifdef DEBUG
    #include <cstdio>    

    extern FILE* debug;
    #define DBG( x, ... ) { fprintf( debug, x"\n", __VA_ARGS__ ); fflush( debug ); }
    #define OPEN_DBG( filename ) { debug = fopen( filename, "w" ); }
    #define CLOSE_DBG() { fclose( debug ); debug = NULL; }
#else
    #define DBG( x, ... )
#endif

// Use only within Serialize methods!
#define SERIALIZE( var )\
    {\
        DBG( "SERIALIZE( " #var " ): %f", static_cast<float>( var ) );\
        buffer = (char*)memcpy( buffer, &var, sizeof( var ) ) + sizeof( var );\
    }
#define SERIALIZE_S( str )\
    {\
        if ( !str ) { size_t n = 0; SERIALIZE( n ); DBG( "SERIALIZE_S( " #str " ): NULL" ); }\
        else {\
            size_t len = strlen( str );\
            SERIALIZE( len );\
            DBG( "SERIALIZE_S( " #str " ): %s", str );\
            buffer = (char*)memcpy( buffer, str, len ) + len;\
        }\
    }

// Use only within Unserialize methods!
#define UNSERIALIZE( var )\
    {\
        memcpy( &var, buffer, sizeof( var ) ); buffer += sizeof( var );\
        DBG( "UNSERIALIZE( " #var " ): %f", static_cast<float>( var ) );\
    }
#define UNSERIALIZE_S( str )\
    {\
        size_t n; UNSERIALIZE( n );\
        if ( n == 0 ) { str = NULL; DBG( "UNSERIALIZE_S( " #str " ): NULL" ); }\
        else {\
            str = new char[n + 1];\
            memcpy( str, buffer, n );\
            str[n] = '\0';\
            DBG( "UNSERIALIZE_S( " #str " ): %s", str );\
        }\
        buffer += n;\
    }

#include "Structures.h"

// *** Global functions ***
IAGSEngine* GetAGS();
Screen const* GetScreen();

#endif // SPRITE3D_COMMON_H
