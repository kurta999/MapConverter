// Converter.cpp : Defines the exported functions for the DLL application.
//

#include "main.h"

logprintf_t logprintf;
extern void *pAMXFunctions;

// native LoadMTAMap(const mapname[], callPawnFunctions);
cell AMX_NATIVE_CALL n_LoadMTAMap(AMX* amx, cell* params)
{
	char *szName;
	amx_StrParam(amx, params[1], szName);

    return CConverter::Get()->LoadMTAMap(std::string(szName), static_cast<int>(params[2]) != 0);
}

// native IsValidMTAMap(mapid);
cell AMX_NATIVE_CALL n_IsValidMTAMap(AMX* amx, cell* params)
{
	return CConverter::Get()->IsValidMTAMap(static_cast<int>(params[1]));
}

// native UnLoadMTAMap(const mapname[]);
cell AMX_NATIVE_CALL n_UnLoadMTAMap(AMX* amx, cell* params)
{
	return CConverter::Get()->UnLoadMTAMap(static_cast<int>(params[1]), static_cast<int>(params[2]) != 0);
}

// native SaveMTAMap(mapid, ConvertingFlags:flags);
cell AMX_NATIVE_CALL n_SaveMTAMap(AMX* amx, cell* params)
{
	return CConverter::Get()->SaveMTAMap(static_cast<int>(params[1]), static_cast<ESavingFlags>(params[2]));
}

// native GetMTAMapName(mapid, name[], len = sizeof(name));
cell AMX_NATIVE_CALL n_GetMTAMapName(AMX* amx, cell* params)
{	
	std::string strMapName = *CConverter::Get()->GetMTAMapName(static_cast<int>(params[1]));
	return set_amxstring(amx, params[2], strMapName.c_str(), params[3]);
}

// native GetMapIDFromName(const mapname[]);
cell AMX_NATIVE_CALL n_GetMapIDFromName(AMX* amx, cell* params)
{	
	char *szName;
	amx_StrParam(amx, params[1], szName);

	return CConverter::Get()->GetMapIDFromName(std::string(szName));
}

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports() 
{
    return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES;
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData) 
{
    pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
    logprintf = (logprintf_t) ppData[PLUGIN_DATA_LOGPRINTF];
	
    logprintf("*** MTA Map Converter v3.0 loaded, by kurta999");
    return true;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
	CConverter::CSingleton::Destroy();

	logprintf("*** MTA Map Converter v3.0 unloaded, by kurta999");
}

AMX_NATIVE_INFO PluginNatives[] =
{
	{"LoadMTAMap",		n_LoadMTAMap},
	{"IsValidMTAMap",	n_IsValidMTAMap},
	{"UnLoadMTAMap",	n_UnLoadMTAMap},
	{"SaveMTAMap",		n_SaveMTAMap},
	{"GetMTAMapName",	n_GetMTAMapName},
	{"GetMapIDFromName",n_GetMapIDFromName},	
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