/**
 * @file D3DSpriteObject.cpp
 * @author Ahonen
 *
 * @brief Sprite object.
 */

#include "D3DSpriteObject.h"
#include <d3dx9.h>

D3DSpriteObject* D3DSpriteObject::Open( long spriteID )
{
    D3DSpriteObject* obj = new D3DSpriteObject();
    obj->myType = TYPE_INTERNAL;
    obj->mySpriteID = spriteID;
    return obj;
}

D3DSpriteObject* D3DSpriteObject::Open( char const* filename, Filtering filtering )
{
    D3DSpriteObject* obj = new D3DSpriteObject();
    obj->myType = TYPE_EXTERNAL;
    obj->myFiltering = filtering;

    // Copy filename
    obj->myFile = new char[ strlen( filename ) + 1 ];
    strcpy( obj->myFile, filename );

    return obj;
}

D3DSpriteObject* D3DSpriteObject::OpenBackground( long frame )
{
	D3DSpriteObject* obj = new D3DSpriteObject();
	obj->myType = TYPE_BACKGROUND;
	obj->mySpriteID = frame;
	return obj;
}

D3DSpriteObject* D3DSpriteObject::Restore( char const* buffer, int size )
{
    D3DSpriteObject* obj = new D3DSpriteObject();
    obj->Unserialize( buffer, size );
    return obj;
}

D3DSpriteObject::D3DSpriteObject():
    D3DObject(),
    myTexture( NULL ),
    myType( TYPE_INTERNAL ),
    mySpriteID( 0 ),
    myData( NULL ),
    myFile( NULL ),
    myTexWidth( 0 ),
    myTexHeight( 0 ),
    myHasAlpha( false )
{
    DBG( "D3DSpriteObject created" );
}

D3DSpriteObject::~D3DSpriteObject()
{
    DBG( "D3DSpriteObject destroyed" );
    
    if ( myTexture )
    {
        myTexture->Release();
        myTexture = NULL;
    }

    if ( myFile )
    {
        delete[] myFile;
        myFile = NULL;
    }
}

int D3DSpriteObject::GetWidth() const
{
    return myWidth;
}

int D3DSpriteObject::GetHeight() const
{
    return myHeight;
}

void D3DSpriteObject::Start()
{
    if ( !myTexture )
    {
        CreateTexture();
    }
}

void D3DSpriteObject::Update()
{
}

void D3DSpriteObject::Render()
{
    // Render texture to screen
    D3DObject::Render( myTexture, myTexWidth, myTexHeight );
}

int D3DSpriteObject::Serialize( char* buffer, int bufsize )
{
    char* bufStart = buffer;

    int num = D3DObject::Serialize( buffer, bufsize );
    buffer += num;
    
    SERIALIZE( myType );
    SERIALIZE( mySpriteID );
    SERIALIZE_S( myFile );
    SERIALIZE( myTexWidth );
    SERIALIZE( myTexHeight );
    SERIALIZE( myHasAlpha );

    return buffer - bufStart;
}

int D3DSpriteObject::Unserialize( char const* buffer, int size )
{
    char const* bufStart = buffer;

    int num = D3DObject::Unserialize( buffer, size );
    buffer += num;

    UNSERIALIZE( myType );
    UNSERIALIZE( mySpriteID );
    UNSERIALIZE_S( myFile );
    UNSERIALIZE( myTexWidth );
    UNSERIALIZE( myTexHeight );
    UNSERIALIZE( myHasAlpha );

    // Load image and create texture
    CreateTexture();

    return buffer - bufStart;
}

void D3DSpriteObject::CreateTexture()
{
    if ( myType == TYPE_INTERNAL && mySpriteID >= 0 )
    {
        // Get sprite from AGS
        DBG( "Creating texture from sprite: %d", mySpriteID );
        BITMAP* bmp = GetAGS()->GetSpriteGraphic( mySpriteID );
        myWidth = GetAGS()->GetSpriteWidth( mySpriteID );
        myHeight = GetAGS()->GetSpriteHeight( mySpriteID );
        myHasAlpha = GetAGS()->IsSpriteAlphaBlended( mySpriteID ) != 0;
        myTexWidth = myWidth;
        myTexHeight = myHeight;

        unsigned char** data = GetAGS()->GetRawBitmapSurface( bmp );

        myTexture = ::CreateTexture( data, myWidth, myHeight, myHasAlpha);
    
        if ( !myTexture )
        {
            DBG( "Could not open sprite #%d", mySpriteID );
        }
    }
	else if ( myType == TYPE_BACKGROUND && mySpriteID >= 0 )
	{
		// Get sprite from AGS background frame
		DBG( "Creating texture from room background: %d", mySpriteID );
		BITMAP* bmp = GetAGS()->GetBackgroundScene( mySpriteID );
		int unused;
		GetAGS()->GetBitmapDimensions( bmp, &myWidth, &myHeight, &unused );
		myHasAlpha = false;
		myTexWidth = myWidth;
		myTexHeight = myHeight;

		unsigned char** data = GetAGS()->GetRawBitmapSurface( bmp );

		myTexture = ::CreateTexture( data, myWidth, myHeight );

		if ( !myTexture )
		{
			DBG( "Could not open room background #%d", mySpriteID );
		}
	}
    else if ( myType == TYPE_EXTERNAL && myFile )
    {
        // Create from PNG data
        DBG( "Creating texture from file: %s", myFile );
        D3DXIMAGE_INFO info;

        // TODO: Tutki paddingia POT:ksi http://subversion.assembla.com/svn/AvP/trunk/xbox/src/d3_func.cpp
        if ( FAILED( D3DXCreateTextureFromFileExA( GetD3D(),
                                                   myFile,
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
                                                   &myTexture ) ) )
        {
            DBG( "Texture creation failed" );
        }
        else
        {
            DBG( "OK" );
            
            // Image size and alpha
            myWidth = info.Width;
            myHeight = info.Height;
            myHasAlpha = info.Format == D3DFMT_A8R8G8B8;

            // Texture size
            D3DSURFACE_DESC desc;
            myTexture->GetLevelDesc( 0, &desc );
            myTexWidth = desc.Width;
            myTexHeight = desc.Height;
        }
    }
}



int D3DSpriteObject_Manager::Dispose( char const* address, bool force )
{
    delete (D3DSpriteObject*)address;
    return 1;
}

char const* D3DSpriteObject_Manager::GetType()
{
    return "D3DSpriteObject";
}

int D3DSpriteObject_Manager::Serialize( char const* address, char* buffer, int bufsize )
{
    return ((D3DSpriteObject*)address)->Serialize( buffer, bufsize );
}

void D3DSpriteObject_Manager::Unserialize( int key, char const* buffer, int size )
{
    D3DSpriteObject* obj = D3DSpriteObject::Restore( buffer, size );

    if ( obj )
    {
        GetAGS()->RegisterUnserializedObject( key, obj, this );
    }
}

