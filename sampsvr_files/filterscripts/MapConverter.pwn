#define FILTERSCRIPT
#include <a_samp>

#include <MapConverter>
#include <streamer>
#tryinclude <YSF>

//#pragma warning disable 217

stock AddVehicleComponentInline(vehicleid, ...)
{
	for (new i = 0, j = numargs(); i != j; ++i)
		AddVehicleComponent(vehicleid, getarg(i));
}

public OnFilterScriptInit()
{
	/*
	new filename[255], i;
	new ret;
	while (ffind("maps/MTA/.*", filename, sizeof(filename), i))
	{
	printf("%d - %s", i, filename);
	ret = MC_LoadMap(filename, 0);
	//MC_SaveMap(ret, CONVERT_ELEMENT_NAME);
	}
	*/
	MC_LoadAllMap(1);
	for (new i = 0; i != MC_GetMapUpperID(); i++)
	{
		if (!MC_IsValidMap(i)) continue;
		MC_SaveMap(i, CONVERT_ELEMENT_NAME);
	}

	//MC_SaveMap(MC_LoadMap("CSAW map1 iceworld", 0), CONVERT_ELEMENT_NAME);

	return 1;
}

public OnMapLoadingStart(mapid, EMapType:maptype, callPawnFunctions)
{
	printf("loading map %d...", mapid);
}

public OnMapLoadingFinish(mapid, EMapType:maptype, callPawnFunctions, objects, removeobjects, vehicles, markers, pickups, actors)
{
	printf("loading map %d finished", mapid);

}
public OnObjectDataLoaded(mapid, modelid, Float:fX, Float:fY, Float:fZ, Float:fRotX, Float:fRotY, Float:fRotZ, interior, virtualworld)
{
	new ret = CreateDynamicObject(modelid, fX, fY, fZ, fRotX, fRotY, fRotZ, interior, virtualworld);
	return ret;
}

public OnObjectDataUnLoaded(mapid, extraid)
{
	DestroyDynamicObject(extraid);
}

public OnRemoveObjectDataLoaded(mapid, modelid, Float:fX, Float:fY, Float:fZ, Float:fRadius)
{

}
public OnRemoveObjectDataUnLoaded(mapid, extraid)
{
	// Do nothing..
}

public OnVehicleDataLoaded(mapid, modelid, Float:fX, Float:fY, Float:fZ, Float:fAngle, color1, color2, paintjob, upgrades[], plate[], interior, virtualworld)
{
	new ret = CreateVehicle(modelid, fX, fY, fZ, fAngle, color1, color2, -1);
	if (paintjob != 3)
		ChangeVehiclePaintjob(ret, paintjob);

	for (new i; i != 14; i++)
	{
		if (upgrades[i])
			AddVehicleComponent(ret, upgrades[i]);
	}
	return ret;
}
public OnVehicleDataUnLoaded(mapid, extraid)
{
	DestroyVehicle(extraid);
}

public OnMarkerDataLoaded(mapid, E_MarkerType:type, Float:fX, Float:fY, Float:fZ, Float:fSize, interior, virtualworld)
{

}
public OnMarkerDataUnLoaded(mapid, extraid)
{

}

public OnPickupDataLoaded(mapid, modelid, Float:fX, Float:fY, Float:fZ, virtualworld)
{
	new ret = CreateDynamicPickup(modelid, 2, fX, fY, fZ, virtualworld);
}
public OnPickupDataUnLoaded(mapid, extraid)
{
	DestroyDynamicPickup(extraid);
}

public OnActorDataLoaded(mapid, modelid, Float:fX, Float:fY, Float:fZ, Float:fAngle, virtualworld)
{
	new ret = CreateActor(modelid, fX, fY, fZ, fAngle);
	if (virtualworld)
		SetActorVirtualWorld(ret, virtualworld);
	return ret;
}
public OnActorDataUnLoaded(mapid, extraid)
{
	DestroyActor(extraid);
}