#include "main.h"
#include "CMapConverter.h"

logprintf_t logprintf;
extern void *pAMXFunctions;

// native MC_LoadMap(const mapname[], callPawnFunctions);
cell AMX_NATIVE_CALL n_MC_LoadMap(AMX* amx, cell* params)
{
	char *szName;
	amx_StrParam(amx, params[1], szName);

    int ret = CConverter::Get()->LoadMap(std::string(szName), static_cast<int>(params[2]) != 0);
	SAFE_DELETE(szName);
	return ret;
}

// native MC_LoadAllMap(callPawnFunctions);
cell AMX_NATIVE_CALL n_MC_LoadAllMap(AMX* amx, cell* params)
{
	return CConverter::Get()->LoadAllMap(static_cast<int>(params[1]) != 0);
}

// native MC_UnloadMap(const mapname[]);
cell AMX_NATIVE_CALL n_MC_UnloadMap(AMX* amx, cell* params)
{
	return CConverter::Get()->UnLoadMTAMap(static_cast<int>(params[1]), static_cast<int>(params[2]) != 0);
}

// native MC_SaveMap(mapid, ConvertingFlags:flags);
cell AMX_NATIVE_CALL n_MC_SaveMap(AMX* amx, cell* params)
{
	return CConverter::Get()->SaveMTAMap(static_cast<int>(params[1]), static_cast<ESavingFlags>(params[2]));
}

// native MC_IsValidMap(mapid);
cell AMX_NATIVE_CALL n_MC_IsValidMap(AMX* amx, cell* params)
{
	return CConverter::Get()->IsValidMap(static_cast<int>(params[1]));
}

// native MC_GetMapUpperID(mapid);
cell AMX_NATIVE_CALL n_MC_GetMapUpperID(AMX* amx, cell* params)
{
	return CConverter::Get()->GetUpperID();
}

// native MC_GetMapName(mapid, name[], len = sizeof(name));
cell AMX_NATIVE_CALL n_MC_GetMapName(AMX* amx, cell* params)
{	
	std::string strMapName = *CConverter::Get()->GetMTAMapName(static_cast<int>(params[1]));
	return set_amxstring(amx, params[2], strMapName.c_str(), params[3]);
}

// native MC_GetIDFromName(const mapname[]);
cell AMX_NATIVE_CALL n_MC_GetIDFromName(AMX* amx, cell* params)
{	
	char *szName;
	amx_StrParam(amx, params[1], szName);

	int ret = CConverter::Get()->GetMapIDFromName(std::string(szName));
	SAFE_DELETE(szName);
	return ret;
}

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports() 
{
    return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES;
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData) 
{
    pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
    logprintf = (logprintf_t) ppData[PLUGIN_DATA_LOGPRINTF];
	
    logprintf("*** Map Converter v3.0 loaded, made by kurta999");
    return true;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
	CConverter::CSingleton::Destroy();

	logprintf("*** Map Converter v3.0, by kurta999");
}

AMX_NATIVE_INFO PluginNatives[] =
{
	{ "MC_LoadMap",			n_MC_LoadMap },
	{ "MC_LoadAllMap",		n_MC_LoadAllMap },
	{ "MC_IsValidMap",		n_MC_IsValidMap },
	{ "MC_UnloadMap",		n_MC_UnloadMap },
	{ "MC_SaveMap",			n_MC_SaveMap },
	{ "MC_IsValidMap",		n_MC_IsValidMap },
	{ "MC_GetMapUpperID",	n_MC_GetMapUpperID },
	{ "MC_GetMapName",		n_MC_GetMapName },
	{ "MC_GetIDFromName",	n_MC_GetIDFromName },
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