#define FILTERSCRIPT
// Last update: 2013.03.09
// Version: 0.3

#include <a_samp>

#include <MapConverter>
#include <YSF>

stock AddVehicleComponentInline(vehicleid, ...)
{
	for(new i = 0, j = numargs(); i != j; ++i)
		AddVehicleComponent(vehicleid, getarg(i));
}

public OnFilterScriptInit()
{

	new filename[255], i;
	new ret;
	while(ffind("maps/MTA/*.*", filename, sizeof(filename), i))
	{
	    printf("%d - %s", i, filename);
	    ret = MC_LoadMap(filename, 0);
	    MC_SaveMap(ret, CONVERT_ELEMENT_NAME);
 	}

    MC_SaveMap(MC_LoadMap("CSAW map1 iceworld", 0), CONVERT_ELEMENT_NAME);

	return 1;
}

public OnMapLoadingStart(mapid, mapname[], EMapType:maptype, callPawnFunctions)
{

}

public OnMapLoadingFinish(mapid, mapname[], EMapType:maptype, callPawnFunctions, objects, removeobjects, vehicles, markers, pickups, actors)
{

}

public OnObjectDataLoaded(mapid, modelid, Float:fX, Float:fY, Float:fZ, Float:fRotX, Float:fRotY, Float:fRotZ, interior, virtualworld)
{

}
public OnObjectDataUnLoaded(mapid, extraid)
{

}

public OnRemoveObjectDataLoaded(mapid, modelid, Float:fX, Float:fY, Float:fZ, Float:fRadius)
{

}
public OnRemoveObjectDataUnLoaded(mapid, extraid)
{

}


public OnVehicleDataLoaded(mapid, modelid, Float:fX, Float:fY, Float:fZ, Float:fAngle, color1, color2, paintjob, upgrades[], plate[], interior, virtualworld)
{

}
public OnVehicleDataUnLoaded(mapid, extraid)
{

}

public OnMarkerDataLoaded(mapid, E_MarkerType:type, Float:fX, Float:fY, Float:fZ, Float:fSize, interior, virtualworld)
{

}
public OnMarkerDataUnLoaded(mapid, extraid)
{

}


public OnPickupDataLoaded(mapid, modelid, Float:fX, Float:fY, Float:fZ, virtualworld)
{

}
public OnPickupDataUnLoaded(mapid, extraid)
{

}

public OnActorDataLoaded(mapid, modelid, Float:fX, Float:fY, Float:fZ, Float:fAngle, interior, virtualworld)
{

}
public OnActorDataUnLoaded(mapid, extraid)
{

}

