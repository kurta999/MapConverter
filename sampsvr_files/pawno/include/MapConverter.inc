#if defined _MapConverter_included
	#endinput
#endif
#define _MapConverter_included
#pragma library MapConverter

// MTA -> SAMP Map Converter v3.0 by kurta999

enum E_SavingFlags
{
	ONLY_CREATE_VEHICLE = 1, // There will be no special data converted ex paintjob, interior and mods. Only CreateVehicle() line
	CONVERT_NUMBER_PLATE = 2, // Convert vehicle number plate?
	HIDE_WHEN_ALPHA_NOT_255 = 4, // If you change the object alpha value different than 255, then hide object in samp? Its useful if you want to create invisible objects in samp
	CONVERT_ELEMENT_NAME = 8 // Convert the name of the element which you give to the element in MTA, useful if you want to find specific model after conversion
};

enum E_MapType
{
	MTA_RACE = 0,
	MTA_DM = 1,
	GTA_IPL = 2	
};

enum E_MarkerType
{
	ARROW,
	CHECKPOINT,
	CORONA,
	CYLINDER,
	RING
};

native MC_LoadMap(const mapname[], callPawnFunctions);
native MC_LoadAllMap(callPawnFunctions);
native MC_UnloadMap(const mapname[]);
native MC_SaveMap(mapid, E_SavingFlags:flags);
native MC_GetMapName(mapid, name[], len = sizeof(name));
native MC_GetIDFromName(const mapname[]);
native MC_IsValidMap(mapid);
native MC_GetMapUpperID();


forward OnMapLoadingStart(mapid, EMapType:maptype, callPawnFunctions);
forward OnMapLoadingFinish(mapid, EMapType:maptype, callPawnFunctions, objects, removeobjects, vehicles, markers, pickups, actors);

forward OnObjectDataLoaded(mapid, modelid, Float:fX, Float:fY, Float:fZ, Float:fRotX, Float:fRotY, Float:fRotZ, interior, virtualworld);
forward OnObjectDataUnLoaded(mapid, extraid);
forward OnRemoveObjectDataLoaded(mapid, modelid, Float:fX, Float:fY, Float:fZ, Float:fRadius);
forward OnRemoveObjectDataUnLoaded(mapid, extraid);
forward OnVehicleDataLoaded(mapid, modelid, Float:fX, Float:fY, Float:fZ, Float:fAngle, color1, color2, paintjob, upgrades[], plate[], interior, virtualworld);
forward OnVehicleDataUnLoaded(mapid, extraid);
forward OnMarkerDataLoaded(mapid, E_MarkerType:type, Float:fX, Float:fY, Float:fZ, Float:fSize, interior, virtualworld);
forward OnMarkerDataUnLoaded(mapid, extraid);
forward OnPickupDataLoaded(mapid, modelid, Float:fX, Float:fY, Float:fZ, virtualworld);
forward OnPickupDataUnLoaded(mapid, extraid);
forward OnActorDataLoaded(mapid, modelid, Float:fX, Float:fY, Float:fZ, Float:fAngle, virtualworld);
forward OnActorDataUnLoaded(mapid, extraid);

