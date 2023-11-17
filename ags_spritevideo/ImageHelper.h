#ifndef SPRITEVIDEO_IMAGEHELPER_H
#define SPRITEVIDEO_IMAGEHELPER_H

#include <vector>

struct ImageInfo
{
    int Width = 0;
    int Height = 0;
    int BPP = 0;
    bool HasAlpha = false;
};

bool LoadImage(const char* file, std::vector<uint8_t> &data, ImageInfo &info);


#endif // SPRITEVIDEO_IMAGEHELPER_H
