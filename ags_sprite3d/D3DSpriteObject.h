/**
 * @file D3DSpriteObject.h
 * @author Ahonen
 *
 * @brief Sprite object.
 */

#ifndef D3D_D3DSPRITEOBJECT_H
#define D3D_D3DSPRITEOBJECT_H

#include "D3DObject.h"

class D3DSpriteObject : public D3DObject
{
public:
    friend class D3DObject;

    static D3DSpriteObject* Open( long spriteID );

    static D3DSpriteObject* Open( char const* filename,
    D3DObject::Filtering filtering = D3DObject::FILTER_LINEAR );

	static D3DSpriteObject* OpenBackground( long frame );

    static D3DSpriteObject* Restore( char const* buffer, int size );

    virtual ~D3DSpriteObject();

    virtual int GetWidth() const;

    virtual int GetHeight() const;

    virtual void Start();

    virtual void Update();

    virtual void Render();

    virtual int Serialize( char* buffer, int bufsize );

    virtual int Unserialize( char const* buffer, int size );

private:
    void CreateTexture();

    enum SpriteType
    {
        TYPE_INTERNAL,   // Create texture with mySpriteID
		TYPE_BACKGROUND, // Create texture with background frame mySpriteID
        TYPE_EXTERNAL    // Create texture with myData
    };

    IDirect3DTexture9* myTexture;

    SpriteType myType;
    long mySpriteID;
    unsigned char* myData;
    char* myFile;

    int myTexWidth;
    int myTexHeight;
    bool myHasAlpha;

    // Blocks
    D3DSpriteObject();
    D3DSpriteObject( D3DSpriteObject const& );
    D3DSpriteObject& operator=( D3DSpriteObject const& );
};


class D3DSpriteObject_Manager : public IAGSScriptManagedObject,
                                public IAGSManagedObjectReader
{
public:
    virtual int Dispose( char const* address, bool force );

    virtual char const* GetType();

    virtual int Serialize( char const* address, char* buffer, int bufsize );

    virtual void Unserialize( int key, char const* buffer, int size );
};

#endif
