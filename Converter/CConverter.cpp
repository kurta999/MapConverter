#include "main.h"
#include <thread>

extern logprintf_t logprintf;
extern CConverter *pConverter;

void StartPawnCompiler(std::string str);

CConverter::CConverter()
{
	mapUpperID = 1;

#ifdef _WIN32
	CreateDirectory("\\scriptfiles\\maps", NULL);
	CreateDirectory("\\scriptfiles\\maps\\SAMP", NULL);
	CreateDirectory("\\scriptfiles\\maps\\MTA", NULL);
#endif
}

CConverter::~CConverter()
{

}

typedef struct quat_s
{
    double w;
    double x;
    double y;
    double z;
} quat_t;

#define pi 3.14159265
#define round(a) ((floor(a)+0.5)<a?ceil(a):floor(a))
#define clamp(a, lo, hi) (a > hi ? hi : (a < lo ? lo : a) )

// Yup2...
inline double rad2deg(double radian)
{
    return radian * (180 / pi);
}

// Yup...
void QuatToEuler(const quat_t *quat, double *rotx,  double *roty, double *rotz)
{
    double sqw;
    double sqx;
    double sqy;
    double sqz;

    double rotxrad;
    double rotyrad;
    double rotzrad;

    sqw = quat->w * quat->w;
    sqx = quat->x * quat->x;
    sqy = quat->y * quat->y;
    sqz = quat->z * quat->z;

    rotxrad = (double)atan2l(2.0 * ( quat->y * quat->z + quat->x * quat->w ) , ( -sqx - sqy + sqz + sqw ));
    rotyrad = (double)asinl( clamp( -2.0 * ( quat->x * quat->z - quat->y * quat->w ), -1, 1 ));
    rotzrad = (double)atan2l(2.0 * ( quat->x * quat->y + quat->z * quat->w ) , (  sqx - sqy - sqz + sqw ));

    *rotx = rad2deg(rotxrad);
    *roty = rad2deg(rotyrad);
    *rotz = rad2deg(rotzrad);

    if (*rotx < 0)
        *rotx = round(360 - *rotx);

    *roty = -(*roty);

    if (*roty < 0)
        *roty = round(360 - *roty);

    if (*rotz < 0)
        *rotz = round(0 - *rotz);
    else if (*rotz > 0)
        *rotz = round(360 - *rotz);

    return;
}

int clampAngle(int angle)
{
	return (angle % 360) + (angle < 0 ? 360 : 0);
}

int CConverter::LoadMTAMap(std::string &strName, bool callPawnFunctions)
{		
	// Map object
	CMap* pMap = NULL;
	int id = mapUpperID++;

	// Remove file type from the end of filename
	size_t pos = strName.find_last_of(".");
	if(pos != std::string::npos)
	{
		if(strName.substr(pos + 1) == "map")
		{
			strName.erase(pos, 4);
		}
	}
	
	// If map already exists with specified name
	if (mapNames.find(strName) != mapNames.end())
		return false;

	pMap = new CMap(strName);	
	maps.emplace(id, pMap);
	mapNames.emplace(strName, id);

	char szPath[MAX_PATH];
	sprintf(szPath, "scriptfiles\\maps\\MTA\\%s.map", strName.c_str());
	
	// Load the map file
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(szPath);
	//logprintf("value: %s", doc.child("map").attribute("mod").value());
	
	// If MTA:DM map
	const pugi::xml_node &mapNode = doc.child("map");
	if(mapNode.attribute("edf:definitions").value()[0] || !strcmp(mapNode.attribute("mod").value(), "deathmatch"))
	{
		//logprintf("mta dm map");

		pMap->mapType = MTA_DM;
		CCallbackManager::OnMapLoadingStart(id, pMap, callPawnFunctions);

		for (auto it : mapNode)
		{
			// objects
			if(!strcmp(it.name(), "object"))
			{
				object_t *object = new object_t;

				object->usModelID = static_cast<WORD>(it.attribute("model").as_int());
				object->vecPos.fX = it.attribute("posX").as_float();
				object->vecPos.fY = it.attribute("posY").as_float();
				object->vecPos.fZ = it.attribute("posZ").as_float();
				object->vecRot.fX = it.attribute("rotX").as_float();
				object->vecRot.fY = it.attribute("rotY").as_float();
				object->vecRot.fZ = it.attribute("rotZ").as_float();
				object->ucInterior = static_cast<BYTE>(it.attribute("interior").as_int());
				object->iWorld = it.attribute("dimension").as_int();
				object->ucAlpha = static_cast<BYTE>(it.attribute("alpha").as_int());
				strcpy(object->szName, it.attribute("id").value());

				// Call our own callback
				if (callPawnFunctions)
					object->extraID = CCallbackManager::OnObjectDataLoaded(id, object);

				pMap->Insert(object);
			}

			// remove objects
			if(!strcmp(it.name(), "removeWorldObject"))
			{
				removeobject_t *removeobject = new removeobject_t;
				
				removeobject->usModelID = static_cast<WORD>(it.attribute("model").as_int());
				WORD usLOD = static_cast<WORD>(it.attribute("lodModel").as_int());
				removeobject->vecPos.fX = it.attribute("posX").as_float();
				removeobject->vecPos.fY = it.attribute("posY").as_float();
				removeobject->vecPos.fZ = it.attribute("posZ").as_float();
				removeobject->fRadius = it.attribute("radius").as_float();

				// Call our own callback
				if (callPawnFunctions)
					removeobject->extraID = CCallbackManager::OnRemoveObjectDataLoaded(id, removeobject);

				pMap->Insert(removeobject);

				// If model has LOD model, then push it too to the vector with same parameters
				if(usLOD)
				{
					// Call our own callback
					if (callPawnFunctions)
						removeobject->extraID = CCallbackManager::OnRemoveObjectDataLoaded(id, removeobject);
					else
						removeobject->extraID = 0;

					pMap->Insert(removeobject);
				}
			}

			// vehicle
			if(!strcmp(it.name(), "vehicle") || !strcmp(it.name(), "spawnpoint"))
			{
				vehicle_t *vehicle = new vehicle_t;

				// Default vehicle format from deathmatch resource
				if(!strcmp(it.name(), "vehicle"))
				{
					vehicle->usModelID = static_cast<WORD>(it.attribute("model").as_int());
					vehicle->vecPos.fX = it.attribute("posX").as_float();
					vehicle->vecPos.fY = it.attribute("posY").as_float();
					vehicle->vecPos.fZ = it.attribute("posZ").as_float();
					vehicle->fAngle = it.attribute("rotZ").as_float();
					strcpy(vehicle->szName, it.attribute("id").value());
				}
				else // Another vehicle format, from race resource in MTA DM
				{
					vehicle->usModelID = static_cast<WORD>(it.attribute("vehicle").as_int());
					vehicle->vecPos.fX = it.attribute("posX").as_float();
					vehicle->vecPos.fY = it.attribute("posY").as_float();
					vehicle->vecPos.fZ = it.attribute("posZ").as_float();
					vehicle->fAngle = it.attribute("rotZ").as_float();
					strcpy(vehicle->szName, it.attribute("name").value());
				}
		
				// If color attribute is present
				if(it.attribute("color"))
				{
					BYTE ucColorByte[12];
					BYTE i = 1;

					// Extract colors from string to array
					ucColorByte[0] = static_cast<BYTE>(atoi(strtok((char*)it.attribute("color").value(), ",")));
					for( ; i != 12; i++)
					{
						char *szRet = strtok(NULL, ",");
						if(!szRet) break;

						ucColorByte[i] = static_cast<BYTE>(atoi(szRet));
						//logprintf("color %d - %d", i, ucColorByte[i]);
					}

					// New MTA - use HEX colors
					if(i > 4)
					{
						// Get GTA color ID from given hex color
						vehicle->iColor1 = (int)CUtils::GetPaletteIndexFromRGB(ucColorByte[0], ucColorByte[1], ucColorByte[2]);
						vehicle->iColor2 = (int)CUtils::GetPaletteIndexFromRGB(ucColorByte[3], ucColorByte[4], ucColorByte[5]);						
					}
					else
					{
						vehicle->iColor1 = ucColorByte[0];
						vehicle->iColor2 = ucColorByte[1];
					}
				}
				else
				{
					vehicle->iColor1 = -1;
					vehicle->iColor2 = -1;
				}

				if(it.attribute("paintjob")) 
					vehicle->ucPaintjob = static_cast<BYTE>(it.attribute("paintjob").as_int());
				else
					vehicle->ucPaintjob = 4;

				// Extract vehicle upgrades from string to array
				memset(vehicle->iUpgrades, 0, sizeof(vehicle->iUpgrades));
				if(it.attribute("upgrades"))
				{
					//logprintf("vantuning geci");
					char szUpgrades[128];
					strcpy(szUpgrades, it.attribute("upgrades").value());
						
					vehicle->iUpgrades[0] = atoi(strtok(szUpgrades, ","));
					for(BYTE i = 1; i != 14; i++)
					{
						char *szRet = strtok(NULL, ",");
						if(!szRet) break;

						vehicle->iUpgrades[i] = atoi(szRet);
						//logprintf("upgrades %d - %d", i, vehicle.iUpgrades[i]);
					}
				}
					
				if(it.attribute("plate"))
					strcpy(vehicle->szPlate, it.attribute("plate").value());
				else
					vehicle->szPlate[0] = NULL;

				// If interior attribute is present
				if(it.attribute("interior")) 
					vehicle->ucInterior = static_cast<BYTE>(it.attribute("interior").as_int());
				else
					vehicle->ucInterior = 0;

				// If dimension attribute is present
				if(it.attribute("dimension"))
					vehicle->iWorld = static_cast<BYTE>(it.attribute("dimension").as_int());
				else
					vehicle->iWorld = 0;

				// Call our own callback
				if (callPawnFunctions)
					vehicle->extraID = CCallbackManager::OnVehicleDataLoaded(id, vehicle);

				pMap->Insert(vehicle);
			}
				
			// marker
			if(!strcmp(it.name(), "marker"))
			{
				marker_t *marker = new marker_t;

				strcpy(marker->szType, it.attribute("type").value());
				marker->vecPos.fX = it.attribute("posX").as_float();
				marker->vecPos.fY = it.attribute("posY").as_float();
				marker->vecPos.fZ = it.attribute("posZ").as_float();
				marker->fSize = it.attribute("size").as_float();
				marker->ucInterior = static_cast<BYTE>(it.attribute("interior").as_int());
				marker->iWorld = it.attribute("dimension").as_int();
				strcpy(marker->szName, it.attribute("id").value());

				// Call our own callback
				if (callPawnFunctions)
					marker->extraID = CCallbackManager::OnCheckpointDataLoaded(id, marker);

				pMap->Insert(marker);
			}

			// pickup
			if(!strcmp(it.name(), "pickup"))
			{
				pickup_t *pickup = new pickup_t;

				pickup->usModelID = static_cast<WORD>(it.attribute("type").as_int());
				pickup->vecPos.fX = it.attribute("posX").as_float();
				pickup->vecPos.fY = it.attribute("posY").as_float();
				pickup->vecPos.fZ = it.attribute("posZ").as_float();
				pickup->iWorld = it.attribute("dimension").as_int();
				strcpy(pickup->szName, it.attribute("id").value());

				// Call our own callback
				if (callPawnFunctions)
					pickup->extraID = CCallbackManager::OnPickupDataLoaded(id, pickup);

				pMap->Insert(pickup);
			}
		}
	}
	else
	{
		object_t object;
		vehicle_t vehicle;

		//logprintf("mta race map");
		pMap->mapType = MTA_RACE;
		CCallbackManager::OnMapLoadingStart(id, pMap, callPawnFunctions);
			
		for (auto it : mapNode)
		{
			// object
			if(!strcmp(it.name(), "object"))
			{
				char *szPos = (char*)it.child_value("model"); 
				if(!szPos) continue;

				object.usModelID = static_cast<WORD>(atoi(szPos));

				if(!(szPos = strtok((char*)it.child_value("position"), " "))) continue;
				object.vecPos.fX = (float)atof(szPos);

				if(!(szPos = strtok(NULL, " "))) continue;
				object.vecPos.fY = (float)atof(szPos);

				if(!(szPos = strtok(NULL, " "))) continue;
				object.vecPos.fZ = (float)atof(szPos);

				if(!(szPos = strtok((char*)it.child_value("rotation"), " "))) continue;
				object.vecRot.fZ = CUtils::RadToDeg((float)atof(szPos));

				if(!(szPos = strtok(NULL, " "))) continue;
				object.vecRot.fY = CUtils::RadToDeg((float)atof(szPos));

				if(!(szPos = strtok(NULL, " "))) continue;
				object.vecRot.fX = CUtils::RadToDeg((float)atof(szPos));

				object.ucInterior = 0;
				object.iWorld = 0;
				object.ucAlpha = 0xFF;
				strcpy(object.szName, it.attribute("name").value());

				// To avoid memory leaks
				object_t *pObject = new object_t;
				memcpy(pObject, &object, sizeof(object_t));

				// Call our own callback
				if (callPawnFunctions)
					pObject->extraID = CCallbackManager::OnObjectDataLoaded(id, pObject);

				pMap->Insert(pObject);
			}

			if(!strcmp(it.name(), "spawnpoint"))
			{
				char *szPos = (char*)it.child_value("vehicle"); 
				if(!szPos) continue;

				vehicle.usModelID = static_cast<WORD>(atoi(szPos));

				if(!(szPos = strtok((char*)it.child_value("position"), " "))) continue;;
				vehicle.vecPos.fX = (float)atof(szPos);

				if(!(szPos = strtok(NULL, " "))) continue;
				vehicle.vecPos.fY = (float)atof(szPos);

				if(!(szPos = strtok(NULL, " "))) continue;
				vehicle.vecPos.fZ = (float)atof(szPos);

				if(!(szPos = (char*)it.child_value("rotation"))) continue;
				vehicle.fAngle = (float)atoi(szPos);

				vehicle.iColor1 = -1;
				vehicle.iColor2 = -1;
				vehicle.ucPaintjob = 4;
				memset(vehicle.iUpgrades, 0, sizeof(vehicle.iUpgrades));
				vehicle.szPlate[0] = NULL;
				vehicle.ucInterior = 0;
				vehicle.iWorld = 0;
				strcpy(vehicle.szName, it.attribute("name").value());

				// To avoid memory leaks
				vehicle_t *pVehicle = new vehicle_t;
				memcpy(pVehicle, &vehicle, sizeof(vehicle_t));
				
				// Call our own callback
				if (callPawnFunctions)
					pVehicle->extraID = CCallbackManager::OnVehicleDataLoaded(id, pVehicle);

				pMap->Insert(pVehicle);				
			}
		}
	}

	CCallbackManager::OnMapLoadingFinish(id, pMap, callPawnFunctions, pMap->vectorObjects.size(), pMap->vectorRemoveObjects.size(), pMap->vectorVehicles.size(), pMap->vectorMarkers.size(), pMap->vectorPickups.size());
	return id;
}

// TODO - Fix rotations
int CConverter::LoadIPL(std::string &strName, bool callPawnFunctions)
{
	// 17512, LODgwforum1_LAe, 0, 2737.75, -1760.0625, 26.2265625, 0, 0, -8.742277657e-008, 1, -1
	// Map object
	CMap* pMap = NULL;
	int id;
	// Remove file type from the end of filename
	size_t pos = strName.find_last_of(".");
	if(pos != std::string::npos)
	{
		std::string ext = strName.substr(pos + 1);
		if(ext == "ipl")
		{
			strName.erase(pos, 4);
		}
	}
	
	// If map isn't loaded
	if(mapNames.find(strName) == mapNames.end())
	{
		pMap = new CMap(strName);		
		maps.emplace(mapUpperID, pMap);
		mapNames.emplace(strName, mapUpperID);

		id = mapUpperID++;
	}
	else
	{
		return false;
	}

	pMap->mapType = GTA_IPL;
	CCallbackManager::OnMapLoadingStart(id, pMap, callPawnFunctions);

	std::ifstream t;
	char szPath2[MAX_PATH];
	sprintf(szPath2, "scriptfiles\\maps\\MTA\\%s.ipl", strName.c_str());
	logprintf(szPath2);

	t.open(szPath2);
	std::string buffer;
	std::string line;
	while(t)
	{
		std::getline(t, line);
		int modelid;
		char szName[64];
		int interior;
		CVector vecPos;
		quat_t quat;
		int lod;
		//logprintf((char*)line.c_str());
		if(sscanf(line.c_str(), "%d, %s %d, %f, %f, %f, %lf, %lf, %lf, %lf, %d", &modelid, &szName, &interior, &vecPos.fX, &vecPos.fY, &vecPos.fZ, 
			&quat.x, &quat.y, &quat.z, &quat.w, &lod))
		{
			logprintf("%d, %s %d, %f, %f, %f, %lf, %lf, %lf, %lf, %d", modelid, szName, interior, vecPos.fX, vecPos.fY, vecPos.fZ, 
			quat.w, quat.x, quat.y, quat.z, lod);

			double rotX, rotY, rotZ;
			QuatToEuler(&quat, &rotX, &rotY, &rotZ);

			object_t* object = new object_t;
			object->usModelID = modelid;
			object->vecPos = vecPos;
			object->vecRot.fX = (float)clampAngle(static_cast<int>(rotX));
			object->vecRot.fY = (float)clampAngle(static_cast<int>(rotY));
			object->vecRot.fZ = (float)clampAngle(static_cast<int>(rotZ));
			object->ucInterior = interior;
			object->iWorld = 0;
			object->ucAlpha = 0xFF;

			// Call our own callback
			if (callPawnFunctions)
				CCallbackManager::OnObjectDataLoaded(id, object);

			pMap->Insert(object);
		}
	}
	CCallbackManager::OnMapLoadingFinish(id, pMap, callPawnFunctions, pMap->vectorObjects.size(), 0, 0, 0, 0);
	t.close();
	return true;
}

bool CConverter::IsValidMTAMap(int mapID)
{
	return maps.find(mapID) != maps.end();
}

bool CConverter::UnLoadMTAMap(int mapID, bool callPawnFunctions)
{
	std::unordered_map<int, CMap*>::iterator map = maps.find(mapID);
	if(map == maps.end())
		return false;

	for(auto o : map->second->vectorObjects)
	{
		if (callPawnFunctions)
			CCallbackManager::OnObjectDataUnLoaded(mapID, o->extraID);

		SAFE_DELETE(o);
	}

	for(auto r : map->second->vectorRemoveObjects)
	{
		if (callPawnFunctions)
			CCallbackManager::OnRemoveObjectDataUnLoaded(mapID, r->extraID);

		SAFE_DELETE(r);
	}

	for(auto v : map->second->vectorVehicles)
	{
		if (callPawnFunctions)
			CCallbackManager::OnVehicleDataUnLoaded(mapID, v->extraID);

		SAFE_DELETE(v);
	}

	for(auto m : map->second->vectorMarkers)
	{
		if (callPawnFunctions)
			CCallbackManager::OnCheckpointDataUnLoaded(mapID, m->extraID);

		SAFE_DELETE(m);
	}

	for(auto p : map->second->vectorPickups)
	{
		if (callPawnFunctions)
			CCallbackManager::OnPickupDataUnLoaded(mapID, p->extraID);

		SAFE_DELETE(p);
	}

	map->second->vectorObjects.clear();
	map->second->vectorRemoveObjects.clear();
	map->second->vectorVehicles.clear();
	map->second->vectorMarkers.clear();
	map->second->vectorPickups.clear();

	SAFE_DELETE(map->second);
	maps.erase(map);
	return true;
}

bool CConverter::SaveMTAMap(int mapID, ESavingFlags flags)
{
	std::unordered_map<int, CMap*>::iterator map = maps.find(mapID);
	if(map == maps.end())
		return false;

	char szDir[MAX_PATH];
	sprintf(szDir, "scriptfiles\\maps\\SAMP\\%s.pwn", map->second->mapName.c_str());
	
	FILE *pfOut = fopen(szDir, "w");
	char szLine[256];
	char szHelp[32];
	bool bVehicleUpgrades = false;
	
	sprintf(szLine, "#include <a_samp>\n#include <streamer>\n// Map Type: %s\n\npublic OnFilterScriptInit()\n{", map->second->mapType == MTA_DM ? "DM" : "Race");
	fputs(szLine, pfOut);
	
	// Add "tempvehid" variable is there are vehicles present
	if (!map->second->vectorVehicles.empty())
	{
		for (auto v : map->second->vectorVehicles)
		{
			if (!(flags & ONLY_CREATE_VEHICLE) && (v->ucInterior || v->iWorld || v->ucPaintjob < 4 || v->iUpgrades[0]))
			{
				fputs("\n\tnew tempvehid; \n\n", pfOut);
				break;
			}
		}
	}

	/////////////////////////////////////////////////
	// Write all objects into the file
	if(!map->second->vectorObjects.empty())
	{
		fputs("\n\t/*--------------------------------------------------Objects -------------------------------------------------*/\n", pfOut);
		for(auto o : map->second->vectorObjects)
		{
			if((flags & HIDE_WHEN_ALPHA_NOT_255) && o->ucAlpha != 0xFF)
			{
				strcpy(szLine, "\tSetDynamicObjectMaterial(CreateDynamicObject(");
			}
			else
			{
				strcpy(szLine, "\tCreateDynamicObject(");
			}

			sprintf(szLine, "%s%d, %f, %f, %f, %f, %f, %f", szLine, o->usModelID, o->vecPos.fX, o->vecPos.fY, o->vecPos.fZ,
				o->vecRot.fX, o->vecRot.fY, o->vecRot.fZ);
			
			// Ha a virtual world, hozzáadjuk a kódhoz
			if(o->iWorld)
			{
				sprintf(szHelp, ", %d", o->iWorld);
				strcat(szLine, szHelp);
			}

			if(o->ucInterior)
			{
				// Ha valamilyen interiorban van az object, de a 0-s virtálus világban - hozzáadjuk a 0s interiort a paraméter sorrend miatt
				if(!o->iWorld)
				{
					strcat(szLine, ", -1"); // Streamer pluginban -1 = minden virtuális világban látható
				}
				sprintf(szHelp, ", %d", o->ucInterior);
				strcat(szLine, szHelp);
			}

			if((flags & HIDE_WHEN_ALPHA_NOT_255) && o->ucAlpha != 0xFF)
			{
				strcat(szLine, "), 0, 0, \"a\", \"a\", 0x000000FF);");
			}
			else
			{
				strcat(szLine, ");");
			}
			strcat(szLine, GetCommect(o->szName, flags));
			fputs(szLine, pfOut);
		}
	}

	/////////////////////////////////////////////////
	// Write all remove objects into the file
	if (!map->second->vectorRemoveObjects.empty())
	{
		fputs("\n\t/*--------------------------------------------------Remove Objects -------------------------------------------------*/\n", pfOut);
		for(auto r : map->second->vectorRemoveObjects)
		{
			sprintf(szLine, "\tRemoveBuildingForPlayer(playerid, %d, %f, %f, %f, %f);\n", r->usModelID, r->vecPos.fX, r->vecPos.fY, r->vecPos.fZ, r->fRadius);
			fputs(szLine, pfOut);
				
		}
	}
	/////////////////////////////////////////////////
	// Write all vehicles into the file
	if (!map->second->vectorVehicles.empty())
	{
		fputs("\n\t/*--------------------------------------------------Vehicles -------------------------------------------------*/\n", pfOut);
		for(auto v : map->second->vectorVehicles)
		{
			if(flags & ONLY_CREATE_VEHICLE || (!v->ucInterior && !v->iWorld && v->ucPaintjob > 2 && !v->iUpgrades[0]))
			{
				sprintf(szLine, "\tCreateVehicle(%d, %f, %f, %f, %f, %d, %d, -1);", v->usModelID, v->vecPos.fX, v->vecPos.fY, v->vecPos.fZ,
					v->fAngle, v->iColor1, v->iColor2);
				strcat(szLine, GetCommect(v->szName, flags));
				fputs(szLine, pfOut);
			}
			else
			{
				sprintf(szLine, "\ttempvehid = CreateVehicle(%d, %f, %f, %f, %f, %d, %d, -1);\n", v->usModelID, v->vecPos.fX, v->vecPos.fY, v->vecPos.fZ,
					v->fAngle, v->iColor1, v->iColor2);
				fputs(szLine, pfOut);

				if(flags & SAVE_NUMBER_PLATE)
				{
					sprintf(szLine, "\tSetVehicleNumberPlate(tempvehid, \"%s\");\n", v->szPlate);
					fputs(szLine, pfOut);
				}

				if(v->ucInterior)
				{
					sprintf(szLine, "\tLinkVehicleToInterior(tempvehid, %d);\n", v->ucInterior);
					fputs(szLine, pfOut);					
				}
					
				if(v->iWorld)
				{
					sprintf(szLine, "\tSetVehicleVirtualWorld(tempvehid, %d);\n", v->iWorld);
					fputs(szLine, pfOut);					
				}

				if(v->ucPaintjob < 3)
				{
					sprintf(szLine, "\tChangeVehiclePaintjob(tempvehid, %d);\n", v->ucPaintjob);
					fputs(szLine, pfOut);					
				}

				if (v->iUpgrades[0])
				{
					if(!bVehicleUpgrades)
						bVehicleUpgrades = true;
					
					sprintf(szLine, "\tAddVehicleComponentInline(tempvehid, ");
					fputs(szLine, pfOut);
					for (BYTE i = 0; i != 14; i++)
					{
						if (i == 13)
							sprintf(szLine, "%d);", v->iUpgrades[i]);
						else
							sprintf(szLine, "%d, ", v->iUpgrades[i]);
						
						fputs(szLine, pfOut);
					}
				}

			}
		}
	}

	/////////////////////////////////////////////////
	// Write all markers into the file
	if (!map->second->vectorMarkers.empty())
	{
		fputs("\n\t/*--------------------------------------------------Checkpoints -------------------------------------------------*/\n", pfOut);
		for(auto m : map->second->vectorMarkers)
		{
			// // "arrow", "checkpoint", "corona", "cylinder", "ring"
			switch(m->szType[1])
			{
				// arrow	
				case 'r':	
				{
					break;
				}
					
				// checkpoint
				case 'h': 
				{
					sprintf(szLine, "\tCreateDynamicCP(%.4f, %.4f, %.4f, %.4f", m->vecPos.fX, m->vecPos.fY, m->vecPos.fZ, m->fSize);
					break;
				}

				// corona
				case 'o':
				{
					break;
				}

				// cylinder (default CP)
				case 'y':
				{
					sprintf(szLine, "\tCreateDynamicRaceCP(2, %.4f, %.4f, %.4f, 0.0, 0.0, 0.0, %.4f", m->vecPos.fX, m->vecPos.fY, m->vecPos.fZ, m->fSize);
					break;
				}

				// ring
				case 'i':
				{
					sprintf(szLine, "\tCreateDynamicRaceCP(4, %.4f, %.4f, %.4f, 0.0, 0.0, 0.0, %.4f", m->vecPos.fX, m->vecPos.fY, m->vecPos.fZ, m->fSize);
					break;
				}
			}

			if(m->iWorld)
			{
				sprintf(szHelp, ", %d", m->iWorld);
				strcat(szLine, szHelp);
			}

			if(m->ucInterior)
			{
				if(!m->iWorld)
					strcat(szLine, ", -1");

				sprintf(szHelp, ", %d", m->ucInterior);
				strcat(szLine, szHelp);
			}

			strcat(szLine, ");");
			strcat(szLine, GetCommect(m->szName, flags));
			fputs(szLine, pfOut);
		}
	}
	/////////////////////////////////////////////////
	// Write all pickups into the file
	if (!map->second->vectorPickups.empty())
	{
		fputs("\n\t/*--------------------------------------------------Pickups -------------------------------------------------*/\n", pfOut);
		for(auto p : map->second->vectorPickups)
		{
			sprintf(szLine, "\tCreateDynamicPickup(%d, 2, %f, %f, %f", CUtils::GetPickupModelIDFromWeaponID((BYTE)p->usModelID), p->vecPos.fX, p->vecPos.fY, p->vecPos.fZ);
			
			if (p->iWorld)
			{
				sprintf(szHelp, ", %d", p->iWorld);
				strcat(szLine, szHelp);
			}

			if (p->ucInterior)
			{
				if (!p->iWorld)
					strcat(szLine, ", -1");

				sprintf(szHelp, ", %d", p->ucInterior);
				strcat(szLine, szHelp);
			}

			strcat(szLine, ");");
			strcat(szLine, GetCommect(p->szName, flags));
			fputs(szLine, pfOut);
		}
	}	
	/*
	char szLocation[MAX_PATH];
	char szCommandLine[MAX_PATH * 3];
	GetCurrentDirectory(MAX_PATH, szLocation);
	
	sprintf(szCommandLine, ".\\pawno\\pawncc.exe %s\\scriptfiles\\maps\\SAMP\\%s.pwn", szLocation, map->second->mapName.c_str());
	std::string cmd = szCommandLine;

	std::thread t1 (StartPawnCompiler, cmd);
	t1.detach();
	*/
	fputs("\treturn 1;\n}", pfOut);

	// If vehicle components has been used then write our own function at the bottom of script
	if(bVehicleUpgrades)
		fputs("\n\nstock AddVehicleComponentInline(vehicleid, ...)\n{\n\tfor (new i = 0, j = numargs(); i != j; ++i)\n\t\tAddVehicleComponent(vehicleid, getarg(i));\n}", pfOut);
	
	logprintf("-> %d : %s.map", map->first, map->second->mapName.c_str());
	logprintf("    %d objects saved", map->second->vectorObjects.size());
	logprintf("    %d remove objects saved", map->second->vectorRemoveObjects.size());
	logprintf("    %d vehicles saved", map->second->vectorVehicles.size());
	logprintf("    %d checkpoints saved", map->second->vectorMarkers.size());
	logprintf("    %d pickups saved\n", map->second->vectorPickups.size());
		
	// Close output file
	fclose(pfOut); 
	return true;
}

void StartPawnCompiler(std::string str)
{
	system(str.c_str());
}

std::string* CConverter::GetMTAMapName(int mapID)
{
	std::unordered_map<int, CMap*>::iterator map = maps.find(mapID);
	if(map == maps.end()) return NULL;

	return &map->second->mapName;
}

int CConverter::GetMapIDFromName(std::string &strMap)
{
	std::unordered_map<std::string, int>::iterator it = mapNames.find(strMap);
	if(it == mapNames.end())
		return -1;

	return it->second;
}

char *CConverter::GetCommect(char *szFromComment, ESavingFlags flags)
{
	static char szRet[64];

	if(!szFromComment || !(flags & CONVERT_ELEMENT_NAME))
	{
		sprintf(szRet, "\n");
	}
	else
	{
		sprintf(szRet, " // %s\n", szFromComment);
	}
	return szRet;
}