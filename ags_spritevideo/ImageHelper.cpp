#include "ImageHelper.h"
#include <png.h>
#include <vector>
#include "Common.h"
#include "StringHelper.h"


// Load image data into the memory, using engine's capabilities
static bool LoadImageData(const char *filename, std::vector<uint8_t> &data)
{
    if (GetAGS()->version < 28)
    {
        // Only reading directly from files on disk supported
        char buf[1024];
        if (GetAGS()->version < 27)
            GetAGS()->GetPathToFileInCompiledFolder(filename, buf);
        else
            GetAGS()->ResolveFilePath(filename, buf, sizeof(buf));
        FILE *f = fopen(buf, "rb");
        if (f)
        {
            // TODO: get stdio compat functions from AGS
            fseek(f, 0, SEEK_END);
            long off = ftell(f);
            fseek(f, 0, SEEK_SET);
            data.resize(off);
            size_t read_num = fread(&data[0], 1, off, f);
            data.resize(read_num);
            fclose(f);
            return true;
        }
    }
    else
    {
        // Support utilizing engine to read from anywhere
        IAGSStream *is = GetAGS()->OpenFileStream(filename, AGSSTREAM_FILE_OPEN, AGSSTREAM_MODE_READ);
        if (is)
        {
            data.resize(is->GetLength());
            size_t read_num = is->Read(&data[0], data.size());
            data.resize(read_num);
            is->Close();
            return true;
        }
    }
    return false;
}

static bool LoadPNG(const std::vector<uint8_t> &raw_bytes, std::vector<uint8_t> &data,
    ImageInfo &info, bool rgba)
{
    // http://www.libpng.org/pub/png/libpng-manual.txt
    // see V. Simplified API
    png_image im;
    memset(&im, 0, sizeof(im));
    im.version = PNG_IMAGE_VERSION;
    if (png_image_begin_read_from_memory(&im, &raw_bytes[0], raw_bytes.size()) == 0)
        return false;
    im.format = rgba ? PNG_FORMAT_RGBA : PNG_FORMAT_BGRA;
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

bool LoadImage(const char* file, std::vector<uint8_t> &data, ImageInfo &info, bool rgba)
{
    const char* ext = GetExt(file);
    if (!ext)
        ext = ".png"; // just try?
    else
        ext++;

    if (stricmp(ext, "png") != 0)
    {
        DBGF("Image format not supported: %s", file);
        return false; // not supported
    }

    std::vector<uint8_t> raw_bytes;
    if (LoadImageData(file, raw_bytes))
        return LoadPNG(raw_bytes, data, info, rgba);
    else
        return false;
}
