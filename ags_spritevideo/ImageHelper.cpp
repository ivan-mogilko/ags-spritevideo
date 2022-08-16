#include "ImageHelper.h"
#include <png.h>
#include <vector>
#include "Common.h"
#include "StringHelper.h"


static bool LoadPNG(const char* file, std::vector<unsigned char> &data, ImageInfo &info)
{
    // http://www.libpng.org/pub/png/libpng-manual.txt
    // see V. Simplified API
    png_image im;
    memset(&im, 0, sizeof(im));
    im.version = PNG_IMAGE_VERSION;
    if (png_image_begin_read_from_file(&im, file) == 0)
        return false;
    im.format = PNG_FORMAT_BGRA;
    const int bpp = PNG_IMAGE_SAMPLE_SIZE(im.format);
    int stride = im.width * bpp;
    data.resize(stride * im.height);
    if (png_image_finish_read(&im, nullptr, &data[0], stride, nullptr) == 0)
        return false;
    info.Width = im.width;
    info.Height = im.height;
    info.BPP = bpp;
    info.HasAlpha = (im.format & PNG_FORMAT_FLAG_ALPHA) != 0;
    return true;
}

bool LoadImage(const char* file, std::vector<unsigned char> &data, ImageInfo &info)
{
    const char* ext = GetExt(file);
    if (!ext)
        ext = ".png"; // just try?
    else
        ext++;

    if (stricmp(ext, "png") == 0)
    {
        return LoadPNG(file, data, info);
    }
    DBGF("Image format not supported: %s", file);
    return false; // not supported
}
