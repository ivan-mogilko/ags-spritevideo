/**
 * @file D3DObject.cpp
 * @author Ahonen
 *
 * @brief Base class for Direct3D objects.
 */

#include "D3DObject.h"
#include <d3dx9.h>
#include "D3DSpriteObject.h"

float const RADS_PER_DEGREE = 3.14159265f / 180.f;
std::list< D3DObject* > D3DObject::ourObjects;
std::list< D3DObject* > D3DObject::ourStartQueue;


void D3DObject::UpdateAll()
{
    for ( auto i = ourObjects.begin(); i != ourObjects.end(); ++i )
    {
        if ( (*i)->myIsEnabled && (*i)->myIsAutoUpdated && (*i)->myHasStarted )
        {
            if ( (*i)->myRoom < 0 || (*i)->myRoom == GetAGS()->GetCurrentRoom() )
            {
                (*i)->Update();
            }
        }
    }
}

void D3DObject::RenderAll( RenderStage stage )
{
    // Run start calls
    for ( auto i = ourStartQueue.begin(); i != ourStartQueue.end(); )
    {
        auto next = i;
        ++next;

        (*i)->Start();
        (*i)->myHasStarted = true;
        ourStartQueue.erase( i );

        i = next;
    }

    // Render
    for ( auto i = ourObjects.begin(); i != ourObjects.end(); ++i )
    {
        if ( (*i)->myIsVisible && (*i)->myIsAutoRendered && (*i)->myRenderStage == stage )
        {
            if ( (*i)->myRoom < 0 || (*i)->myRoom == GetAGS()->GetCurrentRoom() )
            {
                (*i)->Render();
            }
        }
    }
}

D3DObject::D3DObject():
    myHasStarted( false ),
    myIsEnabled( true ),
    myIsVisible( true ),
    myPosition( 0, 0 ),
    myAnchor( 0, 0 ),
    myRotation( 0.f ),
    myScaling( 1.f, 1.f ),
    myIsAutoUpdated( true ),
    myIsAutoRendered( true ),
    myRenderStage( STAGE_BACKGROUND ),
    myRelativeTo( RELATIVE_ROOM ),
    myFiltering( FILTER_LINEAR ),
    myRoom( -1 ),
	myTintR( 1.f ),
	myTintG( 1.f ),
	myTintB( 1.f ),
	myAlpha( 1.f ),
	myParent( NULL ),
	myWidth( 0 ),
	myHeight( 0 )
{
    DBG( "D3DObject created" );
    ourObjects.push_back( this );
    ourStartQueue.push_back( this );
}

D3DObject::~D3DObject()
{
    for ( auto i = ourObjects.begin(); i != ourObjects.end(); ++i )
    {
        if ( *i == this )
        {
            ourObjects.erase( i );
            break;
        }
    }

	for ( auto i = ourStartQueue.begin(); i != ourStartQueue.end(); ++i )
    {
        if ( *i == this )
        {
            ourStartQueue.erase( i );
            break;
        }
    }

    DBG( "D3DObject destroyed" );
}

void D3DObject::SetEnabled( bool enabled )
{
    myIsEnabled = enabled;
}

bool D3DObject::IsEnabled() const
{
    return myIsEnabled;
}

void D3DObject::SetVisible( bool visible )
{
    myIsVisible = visible;
}

bool D3DObject::IsVisible() const
{
    return myIsVisible;
}

void D3DObject::SetPosition( Point const& position )
{
    myPosition = position;
}

Point D3DObject::GetPosition() const
{
    return myPosition;
}

void D3DObject::SetAnchor( PointF const& anchor )
{
    myAnchor = anchor;
}

PointF D3DObject::GetAnchor() const
{
    return myAnchor;
}

void D3DObject::SetRotation( float degrees )
{
    myRotation = degrees;
}

float D3DObject::GetRotation() const
{
    return myRotation;
}

void D3DObject::SetScaling( PointF const& scaling )
{
    myScaling = scaling;
}

void D3DObject::SetScaling( float scaling )
{
    myScaling = PointF( scaling, scaling );
}

PointF D3DObject::GetScaling() const
{
    return myScaling;
}

void D3DObject::SetTintR( float r )
{
	myTintR = r;
}

float D3DObject::GetTintR() const
{
	return myTintR;
}

void D3DObject::SetTintG( float g )
{
	myTintG = g;
}

float D3DObject::GetTintG() const
{
	return myTintG;
}

void D3DObject::SetTintB( float b )
{
	myTintB = b;
}

float D3DObject::GetTintB() const
{
	return myTintB;
}

void D3DObject::SetTint( float r, float g, float b )
{
	myTintR = r;
	myTintG = g;
	myTintB = b;
}

void D3DObject::SetAlpha( float a )
{
	myAlpha = a;
}

float D3DObject::GetAlpha() const
{
	return myAlpha;
}

void D3DObject::SetParent( D3DObject* parent )
{
	myParent = parent;
}

D3DObject* D3DObject::GetParent() const
{
	return myParent;
}

void D3DObject::SetAutoUpdated( bool autoUpdated )
{
    myIsAutoUpdated = autoUpdated;
}

bool D3DObject::IsAutoUpdated() const
{
    return myIsAutoUpdated;
}

void D3DObject::SetAutoRendered( bool autoRendered )
{
    myIsAutoRendered = autoRendered;
}

bool D3DObject::IsAutoRendered() const
{
    return myIsAutoRendered;
}

void D3DObject::SetRenderStage( RenderStage stage )
{
    myRenderStage = stage;
}

D3DObject::RenderStage D3DObject::GetRenderStage() const
{
    return myRenderStage;
}

void D3DObject::SetRelativeTo( RelativeTo relative )
{
    myRelativeTo = relative;
}

D3DObject::RelativeTo D3DObject::GetRelativeTo() const
{
    return myRelativeTo;
}

void D3DObject::SetRoom( int room )
{
    myRoom = room;
}

int D3DObject::GetRoom() const
{
    return myRoom;
}

int D3DObject::GetWidth() const
{
    return 0;
}

int D3DObject::GetHeight() const
{
    return 0;
}

int D3DObject::Serialize( char* buffer, int bufsize )
{
    char* bufStart = buffer;

    SERIALIZE( myHasStarted );
    SERIALIZE( myIsEnabled );
    SERIALIZE( myIsVisible );
    SERIALIZE( myPosition.x );
    SERIALIZE( myPosition.y );
    SERIALIZE( myAnchor.x );
    SERIALIZE( myAnchor.y );
    SERIALIZE( myRotation );
    SERIALIZE( myScaling.x );
    SERIALIZE( myScaling.y );
    SERIALIZE( myIsAutoUpdated );
    SERIALIZE( myIsAutoRendered );
    SERIALIZE( myRenderStage );
    SERIALIZE( myRelativeTo );
    SERIALIZE( myRoom );
    SERIALIZE( myFiltering );
	SERIALIZE( myTintR );
	SERIALIZE( myTintG );
	SERIALIZE( myTintB );
	SERIALIZE( myAlpha );

	int parentKey = GetAGS()->GetManagedObjectKeyByAddress( (char*)myParent );
	SERIALIZE( parentKey );
	
	SERIALIZE( myWidth );
	SERIALIZE( myHeight );

    return buffer - bufStart;
}

int D3DObject::Unserialize( char const* buffer, int size )
{
    char const* bufStart = buffer;

    UNSERIALIZE( myHasStarted );
    UNSERIALIZE( myIsEnabled );
    UNSERIALIZE( myIsVisible );
    UNSERIALIZE( myPosition.x );
    UNSERIALIZE( myPosition.y );
    UNSERIALIZE( myAnchor.x );
    UNSERIALIZE( myAnchor.y );
    UNSERIALIZE( myRotation );
    UNSERIALIZE( myScaling.x );
    UNSERIALIZE( myScaling.y );
    UNSERIALIZE( myIsAutoUpdated );
    UNSERIALIZE( myIsAutoRendered );
    UNSERIALIZE( myRenderStage );
    UNSERIALIZE( myRelativeTo );
    UNSERIALIZE( myRoom );
    UNSERIALIZE( myFiltering );
	UNSERIALIZE( myTintR );
	UNSERIALIZE( myTintG );
	UNSERIALIZE( myTintB );
	UNSERIALIZE( myAlpha );

	int parentKey;
	UNSERIALIZE( parentKey );
	myParent = (D3DObject*)GetAGS()->GetManagedObjectAddressByKey( parentKey );
	
	UNSERIALIZE( myWidth );
	UNSERIALIZE( myHeight );

    return buffer - bufStart;
}

void D3DObject::Render( IDirect3DTexture9* texture, int texWidth, int texHeight )
{
    if ( !myIsVisible || !texture )
    {
        return;
    }


    IDirect3DDevice9* device = GetD3D();

    // Parenting
    Point pos( 0, 0 );
	float rotation;
	PointF scaling( 0, 0 );
	PointF anchorPos( 0, 0 );
	float r, g, b, a;
	HandleParenting( &pos, &rotation, &scaling, &anchorPos, &r, &g, &b, &a );

    device->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	device->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	device->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );

    device->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	device->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	device->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );

    device->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_SELECTARG2 );
    device->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    device->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_CURRENT );

    device->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
    device->SetTextureStageState( 1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
    device->SetTextureStageState( 1, D3DTSS_ALPHAARG2, D3DTA_CURRENT );

    device->SetTextureStageState( 2, D3DTSS_COLOROP, D3DTOP_DISABLE );
    device->SetTextureStageState( 2, D3DTSS_ALPHAOP, D3DTOP_DISABLE );

    device->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2 );

    auto screen = GetScreen();
    float screenScaleX = 1.0;//static_cast<float>(screen->backBufferWidth) / screen->width;
    float screenScaleY = 1.0;//static_cast<float>(screen->backBufferHeight) / screen->height;

    // Engine interface >= 25 provides transform matrixes
    if (screen->matrixValid)
    {
        // Relative to screen always, the rest is handled by matrixes
        pos = screen->FromScreen( pos );

        float orthoSizeX = 2.f / screen->globalProj._11;
        float orthoSizeY = 2.f / screen->globalProj._22;
        screenScaleX = orthoSizeX / screen->width;
        screenScaleY = orthoSizeY / screen->height;
        //DBG("---ORTHO w = %f, h = %f", orthoSizeX, orthoSizeY);
    }
    // Engine interface < 25, manually calc position offset depending on render stage
    // (this does not cover all transformations, but this is as much as we can do)
    else
    {
        if ( myRelativeTo == RELATIVE_ROOM )
        {
            // Relative to room
            pos = GetScreen()->FromRoom( pos );
        }
        else if ( myRelativeTo == RELATIVE_SCREEN )
        {
            // Relative to screen
            pos = GetScreen()->FromScreen( pos );
        }
    }

    //DBG("---RENDER screenScale: %f,%f", screenScaleX, screenScaleY);

    // World matrix, set position, anchor, rotation and scaling
    D3DMATRIX trans, scale, rot, anchor;
    //DBG("---RENDER TRANS: %f,%f", pos.x - screenScaleX * GetScreen()->width / 2.f, pos.y - (1.f + 1.f - screenScaleY) * GetScreen()->height / 2.f);
    SetMatrix( &trans, pos.x - screenScaleX * GetScreen()->width / 2.f,
                       pos.y - (1.f + 1.f - screenScaleY) * GetScreen()->height / 2.f,
                       1, 1 );
    //DBG("---RENDER SCALE: %f,%f", screenScaleX * myWidth * scaling.x, screenScaleY * myHeight * scaling.y);
    SetMatrix( &scale, 0, 0, screenScaleX * myWidth * scaling.x, screenScaleY * myHeight * scaling.y );
        
    SetMatrix( &anchor, -anchorPos.x, anchorPos.y, 1, 1 ); // Mirror Y
    SetMatrixRotation( &rot, rotation * RADS_PER_DEGREE );
    
    D3DMATRIX world;
    // Apply self-transform first
    MatrixMultiply( &world, &anchor, &scale );
    MatrixMultiply( &world, &world, &rot );
    MatrixMultiply( &world, &world, &trans );
    // Apply global world matrix too
    MatrixMultiply(&world, &world, &screen->globalWorld);

    // Set transforms
    device->SetTransform( D3DTS_WORLD, &world );
    device->SetTransform( D3DTS_VIEW, &screen->globalView );

    // Scale texture coordinates
    float scaleU = texWidth / static_cast<float>( myWidth );
    float scaleV = texHeight / static_cast<float>( myHeight );

    D3DMATRIX tex;
    SetMatrix( &tex, .5f, -150, 1, .5f );
    device->SetTransform( D3DTS_TEXTURE1, &tex );
	

    // Store old vertex format
    DWORD oldFVF;
    device->GetFVF( &oldFVF );

    if ( myFiltering == FILTER_LINEAR )
    {
        // Linear texture filtering
        device->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
        device->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
    }
    else if ( myFiltering == FILTER_NEAREST )
    {
        // Nearest neighbor filtering
        device->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
        device->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
    }

	// Use the texture
    device->SetTexture( 0, texture );

    /*if ( mask )
    {
		DBG( "HAS MASK" );
        device->SetTexture( 1, mask->myTexture );
    }
	else
	{
		DBG( "NO MASK" );
		device->SetTexture( 1, NULL );
	}*/

    // Set our vertex format
    device->SetFVF( VERTEX_FVF );

	// Copy of the default quad
	Vertex quad[4];
	memcpy( quad, DEFAULT_QUAD, sizeof( Vertex ) * 4 );

	// Tint and alpha
	D3DCOLOR color = D3DCOLOR_RGBA( static_cast<int>( r * 255 ),
									static_cast<int>( g * 255 ),
									static_cast<int>( b * 255 ),
									static_cast<int>( a * 255 ) );
	quad[0].color = quad[1].color = quad[2].color = quad[3].color = color;

    if ( FAILED( device->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, (void*)quad, sizeof( Vertex ) ) ) )
    {
        DBG( "Draw failed" );
    }
	    
    // Restore old vertex format
    device->SetFVF( oldFVF );
}

void D3DObject::HandleParenting( Point* outPosition, float* outRotation, PointF* outScaling, PointF* outAnchor,
								  float* outTintR, float* outTintG, float* outTintB, float* outAlpha ) const
{
	if ( !myParent )
	{
		// No parent
		*outPosition = myPosition;
		*outRotation = myRotation;
		*outScaling = myScaling;
		*outAnchor = myAnchor;
		*outTintR = myTintR;
		*outTintG = myTintG;
		*outTintB = myTintB;
		*outAlpha = myAlpha;
	}
	else
	{
		// Has parent
		Point parentPos( 0, 0 );
		float parentRot;
		PointF parentScale( 0, 0 );
		PointF parentAnchor( 0, 0 );
		float parentR;
		float parentG;
		float parentB;
		float parentA;

		myParent->HandleParenting( &parentPos, &parentRot, &parentScale, &parentAnchor,
								   &parentR, &parentG, &parentB, &parentA );

		*outRotation = myRotation + parentRot;
		outScaling->x = myScaling.x * parentScale.x;
		outScaling->y = myScaling.y * parentScale.y;
		*outTintR = myTintR * parentR;
		*outTintG = myTintG * parentG;
		*outTintB = myTintB * parentB;
		*outAlpha = myAlpha * parentA;

		outPosition->x = parentPos.x;
		outPosition->y = parentPos.y;

		// Parent's absolute anchor position
		float pax = parentPos.x + parentAnchor.x * myParent->myWidth;
		float pay = parentPos.y + parentAnchor.y * myParent->myHeight;
		//DBG( "Parent absolute: %f, %f", pax, pay );

		// Our absolute anchor position
		float ax = myPosition.x + parentPos.x + myAnchor.x * myWidth;
		float ay = myPosition.y + parentPos.y + myAnchor.y * myHeight;
		//DBG( "Our absolute: %f, %f", ax, ay );

		// Difference
		float dx = pax - ax - myPosition.x;
		float dy = pay - ay - myPosition.y;
		//DBG( "Diff: %f, %f", dx, dy );

		float widthRatio = static_cast<float>( myWidth ) / myParent->myWidth;
		float heightRatio = static_cast<float>( myHeight ) / myParent->myHeight;

		outAnchor->x = myAnchor.x + dx / myWidth + parentAnchor.x * widthRatio;
		outAnchor->y = myAnchor.y + dy / myHeight + parentAnchor.y * heightRatio;
		//DBG( "Anchor: %f, %f", outAnchor->x, outAnchor->y );
	}
}
