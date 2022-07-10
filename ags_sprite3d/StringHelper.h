#ifndef SPRITE3D_STRINGHELPER_H
#define SPRITE3D_STRINGHELPER_H

#include <cstring>
#if !defined (WINDOWS_VERSION)
#include <strings.h>
#endif

#ifndef MAX_PATH
#define MAX_PATH (512)
#endif

#if !defined (WINDOWS_VERSION)
inline int stricmp(const char* a, const char* b) { return strcasecmp(a, b); }
#endif

inline const char* GetExt(const char* filename) { return strrchr(filename, '.'); }

#endif // SPRITE3D_STRINGHELPER_H
