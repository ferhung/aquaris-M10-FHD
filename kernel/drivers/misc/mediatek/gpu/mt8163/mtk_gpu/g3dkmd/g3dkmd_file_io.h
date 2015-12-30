#ifndef _G3DKMD_FILE_IO_H_
#define _G3DKMD_FILE_IO_H_

#include <stdarg.h>

#include "g3dbase_common_define.h"
#include "g3dkmd_define.h"
#include "g3dbase_type.h"

typedef struct _g3dKmdFileIo
{
    void* (*fileOpen)(const char* path);
    void (*fileClose)(void* filp);
    void (*fPrintf)(void* filp, const char* pszFormat, ...);
    void (*vfPrintf)(void* filp, const char* pszFormat, va_list args);
    int (*fileWrite)(void* filp, unsigned char* data, unsigned int size);
    void (*fileRemove)(const char* path);

}g3dKmdFileIo;

typedef enum _g3dKmdFileIoMode
{
    G3DKMD_FILEIO_BUFFERFILE,
    G3DKMD_FILEIO_KFILE
    
}g3dKmdFileIoMode;

extern g3dKmdFileIo *g_pKmdFileIo;


void g3dKmdInitFileIo(void);
void g3dKmdSetFileIo(g3dKmdFileIoMode mode);
void g3dKmdBackupFileIo(void);
void g3dKmdRestoreFileIo(void);


void* g3dKmdFileOpen_bufferfile(const char* path);
void g3dKmdFileClose_bufferfile(void* filp);
void g3dKmdFprintf_bufferfile(void* filp, const char* pszFormat, ...);
void g3dKmdVfprintf_bufferfile(void* filp, const char* pszFormat, va_list args);
int g3dKmdFileWrite_bufferfile(void* filp, unsigned char* data, unsigned int size);
void g3dKmdFileRemove_bufferfile(const char* path);


void* g3dKmdFileOpen_kfile(const char* path);
void g3dKmdFileClose_kfile(void* filp);
void g3dKmdFprintf_kfile(void* filp, const char* pszFormat, ...);
void g3dKmdVfprintf_kfile(void* filp, const char* pszFormat, va_list args);
int g3dKmdFileWrite_kfile(void* filp, unsigned char* data, unsigned int size);
void g3dKmdFileRemove_kfile(const char* path);

#if defined(G3DKMDDEBUG) || defined(G3DKMDDEBUG_ENG)
void g3dKmd_openDebugFile(void);
void g3dKmd_closeDebugFile(void);
void g3dKmdDebugPrint(unsigned int type, const char* format, ...);
#endif

#if defined(G3DKMDDEBUG)
  extern G3Duint g_g3dkmd_debug_flags;
  extern G3Duint g_g3dkmd_debug_loglevel;

  G3Duint *g3dKmd_GetKMDDebugFlagsPtr(void);
  G3Duint *g3dKmd_GetKMDDebugLogLevelPtr(void);

  #define KMDDPFCHECK(flags) (unlikely((g_g3dkmd_debug_loglevel >=((flags)& G3DKMD_LLVL_MASK)) && \
                           ((flags) & g_g3dkmd_debug_flags & G3DKMD_MSG_MASK)))

  #define KMDDPF(type, format, ...) do{ if (KMDDPFCHECK(type)) { g3dKmdDebugPrint((type), "[%s] "format, __FUNCTION__, ##__VA_ARGS__);}} while(0) // std version
//  #define KMDDPF(...) do{ g3dKmdDebugPrint( __VA_ARGS__); } while(0)
#else
   #define KMDDPF(...)
#endif

#if defined(G3DKMDDEBUG_ENG)
   #define KMDDPF_ENG(type, format, ...) do{ g3dKmdDebugPrint(type, "[%s] "format, __FUNCTION__, ##__VA_ARGS__);} while(0) // std version
//   #define KMDDPF_ENG(...) do{ g3dKmdDebugPrint(#__VA_ARGS__); } while(0)
#else
   #define KMDDPF_ENG(...)
#endif

#endif //_G3DKMD_FILE_IO_H_
