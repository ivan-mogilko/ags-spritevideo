/**
 * @file BaseObject.h
 * @author Ahonen
 *
 * @brief Base class for Direct3D objects.
 */

#ifndef SPRITE3D_BASEOBJECT_H
#define SPRITE3D_BASEOBJECT_H

#include <list>
#include "Common.h"
#include "RenderObject.h"

class BaseObject
{
public:
    enum Filtering
    {
        FILTER_NEAREST      = 0,
        FILTER_LINEAR       = 1
    };

    enum RenderStage
    {
        STAGE_BACKGROUND    = 0,
        STAGE_SCENE         = 1,
        STAGE_GUI           = 2,
        STAGE_SCREEN        = 3
    };

    enum RelativeTo
    {
        RELATIVE_ROOM       = 0,
        RELATIVE_SCREEN     = 1
    };

    static void UpdateAll();

    static void RenderAll( RenderStage stage );

    BaseObject();

    virtual ~BaseObject();

    void SetEnabled( bool enabled );

    bool IsEnabled() const;

    void SetVisible( bool visible );

    bool IsVisible() const;

    void SetPosition( Point const& position );

    Point GetPosition() const;

    void SetAnchor( PointF const& pivot );

    PointF GetAnchor() const;

    void SetRotation( float degrees );

    float GetRotation() const;

    void SetScaling( PointF const& scaling );

    void SetScaling( float scaling );

    PointF GetScaling() const;

	void SetTintR( float r );

	float GetTintR() const;

	void SetTintG( float g );

	float GetTintG() const;

	void SetTintB( float b );

	float GetTintB() const;

	void SetTint( float r, float g, float b );

	void SetAlpha( float a );

	float GetAlpha() const;

	void SetParent(BaseObject* parent );

    BaseObject* GetParent() const;

    void SetAutoUpdated( bool autoUpdated );

    bool IsAutoUpdated() const;

    void SetAutoRendered( bool autoRendered );

    bool IsAutoRendered() const;

    void SetRenderStage( RenderStage stage );

    RenderStage GetRenderStage() const;

    void SetRelativeTo( RelativeTo relative );

    RelativeTo GetRelativeTo() const;

    void SetRoom( int room );

    int GetRoom() const;

    virtual int GetWidth() const;

    virtual int GetHeight() const;

    virtual void Start() = 0;

    virtual void Update() = 0;

    virtual void Render() = 0;

    virtual int Serialize( char* buffer, int bufsize );

    virtual int Unserialize( char const* buffer, int size );

protected:
	void HandleParenting( Point* outPosition, float* outRotation, PointF* outScaling, PointF* outAnchor,
						  float* outTintR, float* outTintG, float* outTintB, float* outAlpha ) const;
    void RenderSelf();

    bool myHasStarted;
    bool myIsEnabled;
    bool myIsVisible;
    Point myPosition;
    PointF myAnchor;
    float myRotation;   // Degrees
    PointF myScaling;
    bool myIsAutoUpdated;
    bool myIsAutoRendered;
    RenderStage myRenderStage;
    RelativeTo myRelativeTo;
    int myRoom;

    Filtering myFiltering;

	float myTintR;
	float myTintG;
	float myTintB;
	float myAlpha;
    BaseObject* myParent;

	int myWidth;
	int myHeight;

    RenderObject* myRender;

    static std::list< BaseObject* > ourObjects;
    static std::list< BaseObject* > ourStartQueue;
};

#endif // SPRITE3D_BASEOBJECT_H