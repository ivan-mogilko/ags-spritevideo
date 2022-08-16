#include "BaseObject.h"

std::list< BaseObject* > BaseObject::ourObjects;
std::list< BaseObject* > BaseObject::ourStartQueue;


void BaseObject::UpdateAll()
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

void BaseObject::RenderAll( RenderStage stage )
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

BaseObject::BaseObject()
{
    DBG( "BaseObject created" );
    ourObjects.push_back( this );
    ourStartQueue.push_back( this );
}

BaseObject::~BaseObject()
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

    DBG( "BaseObject destroyed" );
}

void BaseObject::SetEnabled( bool enabled )
{
    myIsEnabled = enabled;
}

bool BaseObject::IsEnabled() const
{
    return myIsEnabled;
}

void BaseObject::SetVisible( bool visible )
{
    myIsVisible = visible;
}

bool BaseObject::IsVisible() const
{
    return myIsVisible;
}

void BaseObject::SetPosition( Point const& position )
{
    myPosition = position;
}

Point BaseObject::GetPosition() const
{
    return myPosition;
}

void BaseObject::SetAnchor( PointF const& anchor )
{
    myAnchor = anchor;
}

PointF BaseObject::GetAnchor() const
{
    return myAnchor;
}

void BaseObject::SetRotation( float degrees )
{
    myRotation = degrees;
}

float BaseObject::GetRotation() const
{
    return myRotation;
}

void BaseObject::SetScaling( PointF const& scaling )
{
    myScaling = scaling;
}

void BaseObject::SetScaling( float scaling )
{
    myScaling = PointF( scaling, scaling );
}

PointF BaseObject::GetScaling() const
{
    return myScaling;
}

void BaseObject::SetTintR( float r )
{
	myTintR = r;
}

float BaseObject::GetTintR() const
{
	return myTintR;
}

void BaseObject::SetTintG( float g )
{
	myTintG = g;
}

float BaseObject::GetTintG() const
{
	return myTintG;
}

void BaseObject::SetTintB( float b )
{
	myTintB = b;
}

float BaseObject::GetTintB() const
{
	return myTintB;
}

void BaseObject::SetTint( float r, float g, float b )
{
	myTintR = r;
	myTintG = g;
	myTintB = b;
}

void BaseObject::SetAlpha( float a )
{
	myAlpha = a;
}

float BaseObject::GetAlpha() const
{
	return myAlpha;
}

void BaseObject::SetParent( BaseObject* parent )
{
	myParent = parent;
}

BaseObject* BaseObject::GetParent() const
{
	return myParent;
}

void BaseObject::SetAutoUpdated( bool autoUpdated )
{
    myIsAutoUpdated = autoUpdated;
}

bool BaseObject::IsAutoUpdated() const
{
    return myIsAutoUpdated;
}

void BaseObject::SetAutoRendered( bool autoRendered )
{
    myIsAutoRendered = autoRendered;
}

bool BaseObject::IsAutoRendered() const
{
    return myIsAutoRendered;
}

void BaseObject::SetRenderStage( RenderStage stage )
{
    myRenderStage = stage;
}

BaseObject::RenderStage BaseObject::GetRenderStage() const
{
    return myRenderStage;
}

void BaseObject::SetRelativeTo( RelativeTo relative )
{
    myRelativeTo = relative;
}

BaseObject::RelativeTo BaseObject::GetRelativeTo() const
{
    return myRelativeTo;
}

void BaseObject::SetRoom( int room )
{
    myRoom = room;
}

int BaseObject::GetRoom() const
{
    return myRoom;
}

int BaseObject::GetWidth() const
{
    return 0;
}

int BaseObject::GetHeight() const
{
    return 0;
}

int BaseObject::Serialize( char* buffer, int bufsize )
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

int BaseObject::Unserialize( char const* buffer, int size )
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
	myParent = (BaseObject*)GetAGS()->GetManagedObjectAddressByKey( parentKey );
	
	UNSERIALIZE( myWidth );
	UNSERIALIZE( myHeight );

    return buffer - bufStart;
}

void BaseObject::RenderSelf()
{
    if ( !myIsVisible || !myRender )
    {
        return;
    }

    // Parenting
    Point pos( 0, 0 );
	float rotation;
	PointF scaling( 0, 0 );
	PointF anchorPos( 0, 0 );
	RGBA rgba;
	HandleParenting( &pos, &rotation, &scaling, &anchorPos, &rgba.r, &rgba.g, &rgba.b, &rgba.a );

    auto screen = GetScreen();

    // Engine interface >= 25 provides transform matrixes
    if (screen->matrixValid)
    {
        // Relative to screen always, the rest is handled by matrixes
        pos = screen->FromScreen( pos );
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

    myRender->Render(pos, scaling, rotation, anchorPos, rgba, myFiltering);
}

void BaseObject::HandleParenting( Point* outPosition, float* outRotation, PointF* outScaling, PointF* outAnchor,
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
		//DBGF( "Parent absolute: %f, %f", pax, pay );

		// Our absolute anchor position
		float ax = myPosition.x + parentPos.x + myAnchor.x * myWidth;
		float ay = myPosition.y + parentPos.y + myAnchor.y * myHeight;
		//DBGF( "Our absolute: %f, %f", ax, ay );

		// Difference
		float dx = pax - ax - myPosition.x;
		float dy = pay - ay - myPosition.y;
		//DBGF( "Diff: %f, %f", dx, dy );

		float widthRatio = static_cast<float>( myWidth ) / myParent->myWidth;
		float heightRatio = static_cast<float>( myHeight ) / myParent->myHeight;

		outAnchor->x = myAnchor.x + dx / myWidth + parentAnchor.x * widthRatio;
		outAnchor->y = myAnchor.y + dy / myHeight + parentAnchor.y * heightRatio;
		//DBGF( "Anchor: %f, %f", outAnchor->x, outAnchor->y );
	}
}
