#define FILTERSCRIPT
// Last update: 2013.03.09
// Version: 0.3

#include <a_samp>

#include <converter>
#include <YSF>

enum ESavingFlags
{
	ONLY_CREATE_VEHICLE = 1,
	SAVE_NUMBER_PLATE = 2,
	HIDE_WHEN_ALPHA_NOT_255 = 4,
	CONVERT_ELEMENT_NAME = 8,
};

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
	    ret = LoadMTAMap(filename, 0);
	    SaveMTAMap(ret, 0);
 	}
	return 1;
}
