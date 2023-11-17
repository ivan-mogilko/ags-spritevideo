#ifndef D3D_AGSDATASOURCE_H
#define D3D_AGSDATASOURCE_H

#if defined (VIDEO_PLAYBACK)

#include "TheoraDataSource.h"

class IAGSStream;

class AGSDataSource : public TheoraDataSource
{
public:
    AGSDataSource(std::string path, IAGSStream *is)
        : _path(path)
        , _is(is)
    {}
    virtual ~AGSDataSource();
	/**
		Reads nBytes bytes from data source and returns number of read bytes.
		if function returns less bytes then nBytes, the system assumes EOF is reached.
	*/
	int read(void* output,int nBytes) override;
	//! returns a string representation of the DataSource, eg 'File: source.ogg'
	std::string toString() override;
	//! position the source pointer to byte_index from the start of the source
	void seek(uint64_t byte_index) override;
	//! return the size of the stream in bytes
	uint64_t getSize() override;
	//! return the current position of the source pointer
	uint64_t getPosition() override;

private:
    std::string _path;
    IAGSStream *_is;
};

#endif // VIDEO_PLAYBACK

#endif // D3D_AGSDATASOURCE_H
