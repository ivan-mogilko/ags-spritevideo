#ifndef D3D_D3DVIDEOOBJECT_H
#define D3D_D3DVIDEOOBJECT_H

#if defined (VIDEO_PLAYBACK)

#include <memory>
#include <theoraplayer/TheoraPlayer.h>
#include "BaseObject.h"


class VideoObject : public BaseObject
{
public:
    static void Initialize();
    static void CleanUp();

    // rgba is a simple flag that tells whether the destination should be
    // RGBA or BGRA format; TODO: make proper constants for this
    static VideoObject* Open( char const* filename, bool rgba );
    static VideoObject* Restore( char const* buffer, int size, bool rgba );

    virtual ~VideoObject();
    
    void SetLooping( bool looping );
    bool IsLooping() const;
    void SetFPS( float fps );
    float GetFPS() const;
    bool NextFrame();
    void Autoplay();
    bool IsAutoplaying() const;
    void StopAutoplay();
    virtual int GetWidth() const;
    virtual int GetHeight() const;
    virtual void Start();
    virtual void Update();
    virtual void Render();

    virtual int Serialize( char* buffer, int bufsize );
    virtual int Unserialize( char const* buffer, int size );

private:
    VideoObject(bool rgba);

    TheoraDataSource *CreateDataSource(char const* filename);
    void UpdateTexture();

    static std::unique_ptr<TheoraVideoManager> videoManager;

    bool _rgba = false; // a hint for the desired pixel format
    TheoraVideoClip* myClip = nullptr; // created & destroyed via an interface
    bool myIsAutoplaying = false;
};



class VideoObject_Manager : public IAGSScriptManagedObject,
                               public IAGSManagedObjectReader
{
public:
    virtual int Dispose( void* address, bool force );
    virtual char const* GetType();
    virtual int Serialize( void* address, char* buffer, int bufsize );
    virtual void Unserialize( int key, char const* buffer, int size );
};

#endif // VIDEO_PLAYBACK

#endif
