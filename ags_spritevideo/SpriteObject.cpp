#include "SpriteObject.h"

SpriteObject::SpriteObject()
{
    DBG("SpriteObject created");
}

SpriteObject::~SpriteObject()
{
    DBG("SpriteObject destroyed");
}

SpriteObject* SpriteObject::Open( int spriteID )
{
    SpriteObject* obj = new SpriteObject();
    obj->myType = TYPE_INTERNAL;
    obj->mySpriteID = spriteID;
    return obj;
}

SpriteObject* SpriteObject::Open( char const* filename, Filtering filtering )
{
    SpriteObject* obj = new SpriteObject();
    obj->myType = TYPE_EXTERNAL;
    obj->myFiltering = filtering;
    obj->myFile = filename;
    return obj;
}

SpriteObject* SpriteObject::OpenBackground( int frame )
{
	SpriteObject* obj = new SpriteObject();
	obj->myType = TYPE_BACKGROUND;
	obj->mySpriteID = frame;
	return obj;
}

SpriteObject* SpriteObject::Restore( char const* buffer, int size )
{
    SpriteObject* obj = new SpriteObject();
    obj->Unserialize( buffer, size );
    return obj;
}

int SpriteObject::GetWidth() const
{
    return myWidth;
}

int SpriteObject::GetHeight() const
{
    return myHeight;
}

void SpriteObject::Start()
{
    if ( !myRender )
    {
        CreateTexture();
    }
}

void SpriteObject::Update()
{
}

void SpriteObject::Render()
{
    // Render texture to screen
    BaseObject::RenderSelf();
}

int SpriteObject::Serialize( char* buffer, int bufsize )
{
    char* bufStart = buffer;

    int num = BaseObject::Serialize( buffer, bufsize );
    buffer += num;
    
    SERIALIZE( myType );
    SERIALIZE( mySpriteID );
    SERIALIZE_STR( myFile );
    int texw = myRender ? myRender->GetTexWidth() : 0;
    int texh = myRender ? myRender->GetTexHeight() : 0;
    int texa = myRender ? myRender->GetHasAlpha() : 0;
    SERIALIZE( texw );
    SERIALIZE( texh );
    SERIALIZE( texa );

    return buffer - bufStart;
}

int SpriteObject::Unserialize( char const* buffer, int size )
{
    char const* bufStart = buffer;

    int num = BaseObject::Unserialize( buffer, size );
    buffer += num;

    UNSERIALIZE( myType );
    UNSERIALIZE( mySpriteID );
    UNSERIALIZE_STR( myFile );
    int texw, texh, texa; // looks like redundant fields
    UNSERIALIZE( texw );
    UNSERIALIZE( texh );
    UNSERIALIZE( texa );

    // Load image and create texture
    CreateTexture();

    return buffer - bufStart;
}

void SpriteObject::CreateTexture()
{
    myRender = GetFactory()->CreateRenderObject();
    if ( myType == TYPE_INTERNAL && mySpriteID >= 0 )
    {
        // Get sprite from AGS
        DBGF( "Creating texture from sprite: %d", mySpriteID );
        myRender->CreateTexture(mySpriteID, -1, nullptr);
    }
    else if ( myType == TYPE_BACKGROUND && mySpriteID >= 0 )
    {
        // Get sprite from AGS background frame
        DBGF( "Creating texture from room background: %d", mySpriteID );
        myRender->CreateTexture(-1, mySpriteID, nullptr);
    }
    else if ( myType == TYPE_EXTERNAL && !myFile.empty() )
    {
        // Create from PNG data
        DBGF( "Creating texture from file: %s", myFile.c_str() );
        myRender->CreateTexture(-1, -1, myFile.c_str());
    }

    if (myRender)
    {
        myWidth = myRender->GetTexWidth();
        myHeight = myRender->GetTexHeight();
        DBGF("myRender created: %d x %d", myWidth, myHeight);
    }
}



int SpriteObject_Manager::Dispose( void* address, bool force )
{
    delete (SpriteObject*)address;
    return 1;
}

char const* SpriteObject_Manager::GetType()
{
    return "SpriteObject";
}

int SpriteObject_Manager::Serialize( void* address, char* buffer, int bufsize )
{
    return ((SpriteObject*)address)->Serialize( buffer, bufsize );
}

void SpriteObject_Manager::Unserialize( int key, char const* buffer, int size )
{
    SpriteObject* obj = SpriteObject::Restore( buffer, size );

    if ( obj )
    {
        GetAGS()->RegisterUnserializedObject( key, obj, this );
    }
}

