#ifndef SPRITE3D_IMAGEHELPER_H
#define SPRITE3D_IMAGEHELPER_H

#include <vector>

struct ImageInfo
{
    int Width = 0;
    int Height = 0;
    int BPP = 0;
    bool HasAlpha = false;
};

bool LoadImage(const char* file, std::vector<unsigned char> &data, ImageInfo &info);


#endif // SPRITE3D_IMAGEHELPER_H
