/**
 * @file VideoObject.cpp
 * @author Ahonen
 *
 * @brief Video object (using libtheoraplayer).
 */

#if defined (VIDEO_PLAYBACK)

#include "VideoObject.h"

// Static variables
std::unique_ptr<TheoraVideoManager> VideoObject::videoManager;


void VideoObject::Initialize()
{
    DBG( "Initializing VideoObject" );
    videoManager.reset(new TheoraVideoManager());
}

void VideoObject::CleanUp()
{
    DBG( "Cleaning up VideoObject" );
    videoManager.reset();
}

VideoObject* VideoObject::Open( char const* filename )
{
    VideoObject* obj = new VideoObject();
	try
	{
		obj->myClip = videoManager->createVideoClip( filename, TH_BGRX );
	}
	catch (...)
	{
		DBGF("File could not be opened: %s", filename);;
	}

    if ( !obj->myClip )
    {
        delete obj;
        return nullptr;
    }

    // No looping by default
    obj->myClip->setAutoRestart( false );

    // Wait until frame cache is filled
    while ( obj->myClip->getNumReadyFrames() != obj->myClip->getNumPrecachedFrames() );
    
	obj->myWidth = obj->myClip->getWidth();
	obj->myHeight = obj->myClip->getHeight();

    return obj;
}

VideoObject* VideoObject::Restore( char const* buffer, int size )
{
    VideoObject* obj = new VideoObject();
    obj->Unserialize( buffer, size );
    return obj;
}

VideoObject::VideoObject():
    BaseObject()
{
    if ( !videoManager )
    {
        Initialize();
    }

    DBG( "VideoObject created" );
}

VideoObject::~VideoObject()
{
    DBG( "VideoObject destroyed" );

    if ( myClip )
    {
        videoManager->destroyVideoClip( myClip );
        myClip = nullptr;
    }
}

void VideoObject::SetLooping( bool looping )
{
    if ( !myClip ) return;

    myClip->setAutoRestart( looping );
}

bool VideoObject::IsLooping() const
{
    if ( !myClip ) return false;

    return myClip->getAutoRestart();
}

void VideoObject::SetFPS( float fps )
{
    if ( !myClip ) return;

    myClip->setPlaybackSpeed( fps / myClip->getFps() );
}

float VideoObject::GetFPS() const
{
    if ( !myClip ) return 0.f;

    return myClip->getPlaybackSpeed() * myClip->getFps();
}

bool VideoObject::NextFrame()
{
    if ( !myClip ) return false;

    myClip->updateToNextFrame();

    return !myClip->isDone();
}

void VideoObject::Autoplay()
{
    myIsAutoplaying = true;
}

bool VideoObject::IsAutoplaying() const
{
    return myIsAutoplaying;
}

void VideoObject::StopAutoplay()
{
    myIsAutoplaying = false;
}

int VideoObject::GetWidth() const
{
    if ( !myClip ) return 0;

    return myClip->getWidth();
}

int VideoObject::GetHeight() const
{
    if ( !myClip ) return 0;

    return myClip->getHeight();
}

void VideoObject::Start()
{

    if ( !myClip ) return;

    // Go to the first frame and get texture
    myClip->updateToNextFrame();
    UpdateTexture();
}

void VideoObject::Update()
{
    if ( !myClip ) return;

    if ( myIsAutoplaying )
    {
        // Autoplay video
        myClip->update( GetScreen()->frameDelay );
    }

    // Change texture if frame has changed
    UpdateTexture();
}

void VideoObject::Render()
{
    if ( !myClip ) return;

    // Render texture to screen
    BaseObject::RenderSelf();
}

int VideoObject::Serialize( char* buffer, int bufsize )
{
    char* bufStart = buffer;

    int num = BaseObject::Serialize( buffer, bufsize );
    buffer += num;
    
    SERIALIZE_STR( myClip->getName() );
    float time = myClip->getTimePosition();
    SERIALIZE( time );
    float speed = myClip->getPlaybackSpeed();
    SERIALIZE( speed );
    bool loop = myClip->getAutoRestart();
    SERIALIZE( loop );
    SERIALIZE( myIsAutoplaying );
    int dummy = 0;
    SERIALIZE(dummy);

    return buffer - bufStart;
}

int VideoObject::Unserialize( char const* buffer, int size )
{
    char const* bufStart = buffer;

    int num = BaseObject::Unserialize( buffer, size );
    buffer += num;

    std::string filename;
    UNSERIALIZE_STR( filename );
    float time;
    UNSERIALIZE( time );
    float speed;
    UNSERIALIZE( speed );
    bool loop;
    UNSERIALIZE( loop );
    UNSERIALIZE( myIsAutoplaying );
    int dummy;
    UNSERIALIZE( dummy );

    // Load video
    myClip = videoManager->createVideoClip( filename, TH_BGRA );

    if ( !myClip )
    {
        DBG( "VideoObject clip not found" );
    }
    else
    {
        // Set video properties
        myClip->setAutoRestart( loop );
        myClip->setPlaybackSpeed( speed );
        myClip->seek( time );

        // Wait until frame cache is filled
        while ( myClip->getNumReadyFrames() != myClip->getNumPrecachedFrames() );

        UpdateTexture();
    }

    return buffer - bufStart;
}

void VideoObject::UpdateTexture()
{    
    if ( !myClip ) return;

    TheoraVideoFrame* frame = myClip->getNextFrame();

    if ( frame )
    {
        // New frame, let's update the texture
        if (!myRender)
            myRender = GetFactory()->CreateRenderObject();
        myRender->CreateTexture(frame->getBuffer(), frame->getWidth(), frame->getHeight(), frame->bpp);
        // Pop frame from queue
        myClip->popFrame();
    }
}




int VideoObject_Manager::Dispose( char const* address, bool force )
{
    delete (VideoObject*)address;
    return 1;
}

char const* VideoObject_Manager::GetType()
{
    return "VideoObject";
}

int VideoObject_Manager::Serialize( char const* address, char* buffer, int bufsize )
{
    return ((VideoObject*)address)->Serialize( buffer, bufsize );
}

void VideoObject_Manager::Unserialize( int key, char const* buffer, int size )
{
    VideoObject* obj = VideoObject::Restore( buffer, size );

    if ( obj )
    {
        GetAGS()->RegisterUnserializedObject( key, obj, this );
    }
}

#endif // VIDEO_PLAYBACK
