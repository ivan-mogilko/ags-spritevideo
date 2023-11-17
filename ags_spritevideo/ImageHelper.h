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

// rgba is a simple flag that tells whether the destination should be
// RGBA or BGRA format; TODO: make proper constants for this
bool LoadImage(const char* file, std::vector<uint8_t> &data, ImageInfo &info, bool rgba);


#endif // SPRITEVIDEO_IMAGEHELPER_H
