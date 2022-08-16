#ifndef SPRITEVIDEO_SPRITEOBJECT_H
#define SPRITEVIDEO_SPRITEOBJECT_H

#include <string>
#include "BaseObject.h"

class SpriteObject : public BaseObject
{
public:
    virtual ~SpriteObject();

    static SpriteObject* Open( int spriteID );
    static SpriteObject* Open( char const* filename,
            BaseObject::Filtering filtering = BaseObject::FILTER_LINEAR );
	static SpriteObject* OpenBackground( int frame );
    static SpriteObject* Restore( char const* buffer, int size );

    virtual int GetWidth() const;
    virtual int GetHeight() const;

    virtual void Start();
    virtual void Update();
    virtual void Render();
    virtual int Serialize( char* buffer, int bufsize );
    virtual int Unserialize( char const* buffer, int size );

private:
    SpriteObject();
    void CreateTexture();

    enum SpriteType
    {
        TYPE_INTERNAL,   // Create texture with mySpriteID
		TYPE_BACKGROUND, // Create texture with background frame mySpriteID
        TYPE_EXTERNAL    // Create texture with myFile
    };

    SpriteType myType = TYPE_INTERNAL;
    int mySpriteID = 0;
    std::string myFile;
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

#endif // SPRITEVIDEO_SPRITEOBJECT_H
