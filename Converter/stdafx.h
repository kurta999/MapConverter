// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

typedef void (*logprintf_t)(char* format, ...);

typedef unsigned short ushort;
typedef unsigned char uchar;

#include "CCallbackManager.h"
#include "CConverter.h"
#include "CVector.h"
#include "CStructs.h"
#include "CUtils.h"

// TODO: reference additional headers your program requires here
