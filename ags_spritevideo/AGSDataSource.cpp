#if defined (VIDEO_PLAYBACK)

#include "AGSDataSource.h"
#include "Common.h"

AGSDataSource::~AGSDataSource()
{
    _is->Dispose();
}

int AGSDataSource::read(void* output, int nBytes)
{
    return _is->Read(output, nBytes);
}

std::string AGSDataSource::toString()
{
    return _path;
}

void AGSDataSource::seek(uint64_t byte_index)
{
    _is->Seek(byte_index, 0);
}
	
uint64_t AGSDataSource::getSize()
{
    return _is->GetLength();
}
	
uint64_t AGSDataSource::getPosition()
{
    return _is->GetPosition();
}

#endif // VIDEO_PLAYBACK
