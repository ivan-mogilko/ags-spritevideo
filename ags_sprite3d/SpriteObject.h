/**
 * @file SpriteObject.h
 * @author Ahonen
 *
 * @brief Sprite object.
 */

#ifndef SPRITE3D_SPRITEOBJECT_H
#define SPRITE3D_SPRITEOBJECT_H

#include "BaseObject.h"

class SpriteObject : public BaseObject
{
public:
    friend class BaseObject;

    static SpriteObject* Open( long spriteID );

    static SpriteObject* Open( char const* filename,
    BaseObject::Filtering filtering = BaseObject::FILTER_LINEAR );

	static SpriteObject* OpenBackground( long frame );

    static SpriteObject* Restore( char const* buffer, int size );

    virtual ~SpriteObject();

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

    SpriteType myType;
    long mySpriteID;
    unsigned char* myData;
    char* myFile;

    // Blocks
    SpriteObject();
    SpriteObject( SpriteObject const& );
    SpriteObject& operator=( SpriteObject const& );
};


class SpriteObject_Manager : public IAGSScriptManagedObject,
                             public IAGSManagedObjectReader
{
public:
    virtual int Dispose( char const* address, bool force );

    virtual char const* GetType();

    virtual int Serialize( char const* address, char* buffer, int bufsize );

    virtual void Unserialize( int key, char const* buffer, int size );
};

#endif // SPRITE3D_SPRITEOBJECT_H
