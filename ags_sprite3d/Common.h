/**
 * @file Common.h
 * @author Ahonen
 *
 * @brief Common stuff. Includes, debug, etc.
 */

#ifndef D3D_COMMON_H
#define D3D_COMMON_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// Plugin API
#include "agsplugin.h"

// Direct3D 9.0
#include <d3d9.h>

// Disabled warnings
#pragma warning(disable : 4251) // dll string warnings
#pragma warning(disable : 4996) // sprintf_s etc.

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
IDirect3DDevice9* GetD3D();
IDirect3DPixelShader9* GetPixelShader();

struct ID3DXEffect;
ID3DXEffect* GetFX();

// Matrix functions
void SetMatrix( D3DMATRIX* matrix, float tx, float ty, float sx, float sy );
void SetMatrixIdentity( D3DMATRIX* matrix );
void SetMatrixRotation( D3DMATRIX* matrix, float radians );
void MatrixMultiply( D3DMATRIX* result, const D3DMATRIX* a, const D3DMATRIX* b );

// Texture creation
IDirect3DTexture9* CreateTexture( unsigned char const* data, int width, int height, bool alpha = false );
bool SetTextureData( IDirect3DTexture9* texture, unsigned char const* data, int width, int height );
IDirect3DTexture9* CreateTexture( unsigned char const* const* data, int width, int height, bool alpha = false );
bool SetTextureData( IDirect3DTexture9* texture, unsigned char const* const* data, int width, int height );

#endif
