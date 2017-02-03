// Internal includes
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <unordered_map>

// Project files
#include "CCallbackManager.h"
#include "CConverter.h"
#include "CUtils.h"
#include "CVector.h"
#include "CSingleton.h"

// SAMP SDK
#include "SDK/plugin.h"

// PUGI XML
#include "pugixml.hpp"

typedef void (*logprintf_t)(char* format, ...);

// Externals
extern logprintf_t logprintf;

// Defines
#define NUMELMS(aa) (sizeof(aa) / sizeof((aa)[0]))
#define SAFE_DELETE(p)	{ if (p) { delete (p); (p) = nullptr; } }