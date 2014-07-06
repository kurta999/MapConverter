// Converter.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "SDK/plugin.h"
#include <stdio.h>
#include <iostream>

#pragma warning (disable : 4996)

logprintf_t logprintf;
extern void *pAMXFunctions;

CConverter *pConverter = NULL;

// native ConvertMTAMapFile(path[], flags);
cell AMX_NATIVE_CALL n_ConvertMTAMapFile(AMX* amx, cell* params)
{
	char *szPath;
	amx_StrParam(amx, params[1], szPath);

	pConverter->ConvertMTAMapToSAMP(szPath, static_cast<EConvertingFlags>(params[2]));
    return 1;
}

// native SetMapVehiclesRespawn(time);
cell AMX_NATIVE_CALL n_SetMapVehiclesRespawn(AMX* amx, cell* params)
{
	pConverter->SetVehiclesRespawnDelay((int)params[1]);
    return 1;
}

// native GetMapVehiclesRespawn(time);
cell AMX_NATIVE_CALL n_GetMapVehiclesRespawn(AMX* amx, cell* params)
{
    return pConverter->GetVehiclesRespawnDelay();
}

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports() 
{
    return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES;
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData) 
{
    pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
    logprintf = (logprintf_t) ppData[PLUGIN_DATA_LOGPRINTF];
	
	// Crate converter
	pConverter = new CConverter();

    logprintf("*** MTA Map converter v1.0 loaded, by kurta999");
    return true;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
	delete pConverter;
	pConverter = NULL;
	logprintf("*** MTA Map converter v1.0 unloaded, by kurta999");
}

AMX_NATIVE_INFO PluginNatives[] =
{
	{"ConvertMTAMapFile", n_ConvertMTAMapFile},
	{"SetMapVehiclesRespawn", n_SetMapVehiclesRespawn},
	{"GetMapVehiclesRespawn", n_GetMapVehiclesRespawn},
	{0, 0}
};

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad( AMX *amx ) 
{
	// Register the AMX
	CCallbackManager::RegisterAMX(amx);
	// Register the plugin natives for the amx instance
	return amx_Register(amx, PluginNatives, -1);
}

PLUGIN_EXPORT int PLUGIN_CALL AmxUnload( AMX *amx ) 
{
	// Unregister the AMX
	CCallbackManager::UnregisterAMX(amx);
	return AMX_ERR_NONE;
}