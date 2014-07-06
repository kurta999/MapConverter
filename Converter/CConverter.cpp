#include "stdafx.h"
#include "pugixml.hpp"
#include <stdio.h>
#include <iostream>
#include <fstream>

extern logprintf_t logprintf;
extern CConverter *pConverter;

#pragma warning (disable : 4996)

CConverter::CConverter()
{
	m_iRespawnDelay = 60;
	m_szWorkingFilename = new char[64];
}

CConverter::~CConverter()
{
	delete m_szWorkingFilename;
}

char *strdel(char *string, size_t first, size_t len)
{
      char *pos0, *pos1;

      if (string)
      {
            if (first < strlen(string))
            {
                  for (pos0 = pos1 = string + first;
                        *pos1 && len;
                        ++pos1, --len)
                  {
                        ;
                  }
                  strcpy(pos0, pos1);
            }
      }
      return string;
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
bool CConverter::ConvertMTAMapToSAMP(char *szPath, EConvertingFlags flags)
{
	//logprintf("szPath: %s, flags: %d", szPath, flags & HIDE_WHEN_ALPHA_NOT_255);
	// XML parser objects
	pugi::xml_document doc;
    pugi::xml_parse_result result;
	pugi::xml_node tools;

	// Make path
	char szDir[MAX_PATH];
	uchar len;
	sprintf(szDir, "%s\\MTA\\*.*", szPath);
	//logprintf("szDir: %s", szDir);

	// Check each file in given directory
	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	hFind = FindFirstFile(szDir, &ffd);
	do
	{
		// If lenght is too small - skip
		if((len = strlen(ffd.cFileName)) < 4) 
			continue;

		// Create objects
		object_t object;
		removeobject_t removeobject;
		vehicle_t vehicle;
		marker_t marker;
		pickup_t pickup;
		/*
		// TODO - currently rotations are bad in some situations
		// 17512, LODgwforum1_LAe, 0, 2737.75, -1760.0625, 26.2265625, 0, 0, -8.742277657e-008, 1, -1
		if(ffd.cFileName[len - 4] == '.' && ffd.cFileName[len - 3] == 'i' && ffd.cFileName[len - 2] == 'p' && ffd.cFileName[len - 1] == 'l')
		{
			//logprintf("tefasz");
			std::ifstream t;
			char szPath2[MAX_PATH];
			sprintf(szPath2, "%s\\MTA\\%s", szPath, ffd.cFileName);
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

					object.usModelID = modelid;
					object.vecPos = vecPos;
					object.vecRot.fX = (float)clampAngle(static_cast<int>(rotX));
					object.vecRot.fY = (float)clampAngle(static_cast<int>(rotY));
					object.vecRot.fZ = (float)clampAngle(static_cast<int>(rotZ));
					object.ucInterior = interior;
					object.iWorld = 0;
					object.ucAlpha = 0xFF;

					m_vectorObjects.push_back(object);
				}
			}
			t.close();
		}
		*/
		// if file type is not "map", then continue
		if(ffd.cFileName[len - 4] != '.' || ffd.cFileName[len - 3] != 'm' || ffd.cFileName[len - 2] != 'a' || ffd.cFileName[len - 1] != 'p')
			continue;

		//logprintf("megvan bazmeg");
		// Parse XML file

		char szPath2[MAX_PATH];
		sprintf(szPath2, "%s\\MTA\\%s", szPath, ffd.cFileName);
		result = doc.load_file(szPath2);

		strcpy(m_szWorkingFilename, ffd.cFileName);
		//logprintf("m_szWorkingFilename: %s", m_szWorkingFilename);

		// If MTA:DM map
		if(doc.child("map").attribute("edf:definitions").value()[0])
		{
			m_eMapType = MTA_DM;
			CCallbackManager::OnConvertationStart(m_szWorkingFilename, m_eMapType, flags);

			tools = doc.child("map");
			for (pugi::xml_node_iterator it = tools.begin(); it != tools.end(); ++it)
			{
				// objects
				if(!strcmp(it->name(), "object"))
				{
					object.usModelID = atoi(it->attribute("model").value());
					object.vecPos.fX = (float)atof(it->attribute("posX").value());
					object.vecPos.fY = (float)atof(it->attribute("posY").value());
					object.vecPos.fZ = (float)atof(it->attribute("posZ").value());
					object.vecRot.fX = (float)atof(it->attribute("rotX").value());
					object.vecRot.fY = (float)atof(it->attribute("rotY").value());
					object.vecRot.fZ = (float)atof(it->attribute("rotZ").value());
					object.ucInterior = atoi(it->attribute("interior").value());
					object.iWorld = atoi(it->attribute("dimension").value());
					object.ucAlpha = atoi(it->attribute("alpha").value());
					strcpy(object.szName, it->attribute("id").value());

					m_vectorObjects.push_back(object);
				}

				// remove objects
				if(!strcmp(it->name(), "removeWorldObject"))
				{
					removeobject.usModelID = atoi(it->attribute("model").value());
					ushort usLOD = atoi(it->attribute("lodModel").value());
					removeobject.vecPos.fX = (float)atof(it->attribute("posX").value());
					removeobject.vecPos.fY = (float)atof(it->attribute("posY").value());
					removeobject.vecPos.fZ = (float)atof(it->attribute("posZ").value());
					removeobject.fRadius = (float)atof(it->attribute("radius").value());

					m_vectorRemoveObjects.push_back(removeobject);

					// If model has LOD model, then push it too to the vector with same parameters
					if(usLOD)
					{
						removeobject.usModelID = usLOD;
						m_vectorRemoveObjects.push_back(removeobject);
					}
				}

				// vehicle
				if(!strcmp(it->name(), "vehicle") || !strcmp(it->name(), "spawnpoint"))
				{
					// Default vehicle format from deathmatch resource
					if(!strcmp(it->name(), "vehicle"))
					{
						vehicle.usModelID = atoi(it->attribute("model").value());
						vehicle.vecPos.fX = (float)atof(it->attribute("posX").value());
						vehicle.vecPos.fY = (float)atof(it->attribute("posY").value());
						vehicle.vecPos.fZ = (float)atof(it->attribute("posZ").value());
						vehicle.fAngle = (float)atof(it->attribute("rotZ").value());
						strcpy(vehicle.szName, it->attribute("id").value());
					}
					else // Another vehicle format, from race resource in MTA DM
					{
						vehicle.usModelID = atoi(it->attribute("vehicle").value());
						vehicle.vecPos.fX = (float)atof(it->attribute("posX").value());
						vehicle.vecPos.fY = (float)atof(it->attribute("posY").value());
						vehicle.vecPos.fZ = (float)atof(it->attribute("posZ").value());
						vehicle.fAngle = (float)atof(it->attribute("rotZ").value());
						strcpy(object.szName, it->attribute("name").value());
					}
		
					// If color attribute is present
					if(it->attribute("color"))
					{
						uchar ucColorByte[12];
						uchar i = 1;

						// Extract colors from string to array
						ucColorByte[0] = atoi(strtok((char*)it->attribute("color").value(), ","));
						for( ; i != 12; i++)
						{
							char *szRet = strtok(NULL, ",");
							if(!szRet) break;

							ucColorByte[i] = atoi(szRet);
							//logprintf("color %d - %d", i, ucColorByte[i]);
						}

						// New MTA - use HEX colors
						if(i > 4)
						{
							// Get GTA color ID from given hex color
							vehicle.iColor1 = (int)CUtils::GetPaletteIndexFromRGB(ucColorByte[0], ucColorByte[1], ucColorByte[2]);
							vehicle.iColor2 = (int)CUtils::GetPaletteIndexFromRGB(ucColorByte[3], ucColorByte[4], ucColorByte[5]);						
						}
						else
						{
							vehicle.iColor1 = ucColorByte[0];
							vehicle.iColor2 = ucColorByte[1];
						}
					}
					else
					{
						vehicle.iColor1 = -1;
						vehicle.iColor1 = -1;
					}

					if(it->attribute("paintjob")) 
						vehicle.ucPaintjob = atoi(it->attribute("paintjob").value());
					else
						vehicle.ucPaintjob = 4;

					// Extract vehicle upgrades from string to array
					memset(vehicle.iUpgrades, 0, sizeof(vehicle.iUpgrades));
					if(it->attribute("upgrades"))
					{
						//logprintf("vantuning geci");
						char szUpgrades[128];
						strcpy(szUpgrades, it->attribute("upgrades").value());
						
						vehicle.iUpgrades[0] = atoi(strtok(szUpgrades, ","));
						for(uchar i = 1; i != 14; i++)
						{
							char *szRet = strtok(NULL, ",");
							if(!szRet) break;

							vehicle.iUpgrades[i] = atoi(szRet);
							//logprintf("upgrades %d - %d", i, vehicle.iUpgrades[i]);
						}
					}
					
					if(it->attribute("plate"))
						strcpy(vehicle.szPlate, it->attribute("plate").value());
					else
						vehicle.szPlate[0] = NULL;

					// If interior attribute is present
					if(it->attribute("interior")) 
						vehicle.ucInterior = atoi(it->attribute("interior").value());
					else
						vehicle.ucInterior = 0;

					// If dimension attribute is present
					if(it->attribute("dimension"))
						vehicle.iWorld = atoi(it->attribute("dimension").value());
					else
						vehicle.iWorld = 0;

					m_vectorVehicles.push_back(vehicle);
				}
				
				// marker
				if(!strcmp(it->name(), "marker"))
				{
					strcpy(marker.szType, it->attribute("type").value());
					marker.vecPos.fX = (float)atof(it->attribute("posX").value());
					marker.vecPos.fY = (float)atof(it->attribute("posY").value());
					marker.vecPos.fZ = (float)atof(it->attribute("posZ").value());
					marker.fSize = (float)atof(it->attribute("size").value());
					marker.ucInterior = atoi(it->attribute("interior").value());
					marker.iWorld = atoi(it->attribute("dimension").value());
					strcpy(marker.szName, it->attribute("id").value());

					m_vectorMarkers.push_back(marker);
				}

				// pickup
				if(!strcmp(it->name(), "pickup"))
				{
					pickup.usModelID = atoi(it->attribute("type").value());
					pickup.vecPos.fX = (float)atof(it->attribute("posX").value());
					pickup.vecPos.fY = (float)atof(it->attribute("posY").value());
					pickup.vecPos.fZ = (float)atof(it->attribute("posZ").value());
					pickup.iWorld = atoi(it->attribute("type").value());
					strcpy(pickup.szName, it->attribute("id").value());

					m_vectorPickups.push_back(pickup);
				}
			}
		}
		else
		{
			m_eMapType = MTA_RACE;
			CCallbackManager::OnConvertationStart(m_szWorkingFilename, m_eMapType, flags);
			
			tools = doc.child("map");
			for (pugi::xml_node_iterator it = tools.begin(); it != tools.end(); ++it)
			{
				// object
				if(!strcmp(it->name(), "object"))
				{
					char *szPos = (char*)it->child_value("model"); 
					if(!szPos) continue;

					object.usModelID = atoi(szPos);

					if(!(szPos = strtok((char*)it->child_value("position"), " "))) continue;;
					object.vecPos.fX = (float)atof(szPos);

					if(!(szPos = strtok(NULL, " "))) continue;
					object.vecPos.fY = (float)atof(szPos);

					if(!(szPos = strtok(NULL, " "))) continue;
					object.vecPos.fZ = (float)atof(szPos);

					if(!(szPos = strtok((char*)it->child_value("rotation"), " "))) continue;;
					object.vecRot.fZ = CUtils::RadToDeg((float)atof(szPos));

					if(!(szPos = strtok(NULL, " "))) continue;
					object.vecRot.fY = CUtils::RadToDeg((float)atof(szPos));

					if(!(szPos = strtok(NULL, " "))) continue;
					object.vecRot.fX = CUtils::RadToDeg((float)atof(szPos));

					object.ucInterior = 0;
					object.iWorld = 0;
					object.ucAlpha = 0xFF;
					object.szName[0] = NULL; // Sorry, wait for v1.1

					m_vectorObjects.push_back(object);
				}

				if(!strcmp(it->name(), "spawnpoint"))
				{
					char *szPos = (char*)it->child_value("vehicle"); 
					if(!szPos) continue;

					vehicle.usModelID = atoi(szPos);

					if(!(szPos = strtok((char*)it->child_value("position"), " "))) continue;;
					vehicle.vecPos.fX = (float)atof(szPos);

					if(!(szPos = strtok(NULL, " "))) continue;
					vehicle.vecPos.fY = (float)atof(szPos);

					if(!(szPos = strtok(NULL, " "))) continue;
					vehicle.vecPos.fZ = (float)atof(szPos);

					if(!(szPos = (char*)it->child_value("rotation"))) continue;
					vehicle.fAngle = (float)atoi(szPos);

					vehicle.iColor1 = -1;
					vehicle.iColor2 = -1;
					vehicle.ucPaintjob = 4;
					memset(vehicle.iUpgrades, 0, sizeof(vehicle.iUpgrades));
					vehicle.szPlate[0] = NULL;
					vehicle.ucInterior = 0;
					vehicle.iWorld = 0;
					vehicle.szName[0] = NULL; // Sorry, wait for v1.1

					m_vectorVehicles.push_back(vehicle);
				}
			}
			
		}
		
		// Write
		/////////////////////////////////////////////////
		//strcpy(szDir, ffd.cFileName);
		strdel(ffd.cFileName, len - 4, len);
		sprintf(szDir, "%s\\SAMP\\%s.pwn", szPath, ffd.cFileName);
		//logprintf("after strdel: %s", szDir);
	
		FILE *pfOut = fopen(szDir, "w");
		char szLine[256];
		char szHelp[32];

		fputs("/*--------------------------------------------------Objects -------------------------------------------------*/\n", pfOut);
		/////////////////////////////////////////////////
		// Write all objects into the file
		{
			for(std::vector<object_t>::iterator object = m_vectorObjects.begin(); object != m_vectorObjects.end(); ++object)
			{
				// Call your callback
				if(!(flags & NO_PAWN_CALLS))
					CCallbackManager::OnObjectDataConverted(*object);

				if((flags & HIDE_WHEN_ALPHA_NOT_255) && object->ucAlpha != 0xFF)
				{
					strcpy(szLine, "SetDynamicObjectMaterial(CreateDynamicObject(");
				}
				else
				{
					strcpy(szLine, "CreateDynamicObject(");
				}

				sprintf(szLine, "%s%d, %f, %f, %f, %f, %f, %f", szLine, object->usModelID, object->vecPos.fX, object->vecPos.fY, object->vecPos.fZ,
					object->vecRot.fX, object->vecRot.fY, object->vecRot.fZ);
			
				// Ha a virtual world, hozzáadjuk a kódhoz
				if(object->iWorld)
				{
					sprintf(szHelp, ", %d", object->iWorld);
					strcat(szLine, szHelp);
				}

				if(object->ucInterior)
				{
					// Ha valamilyen interiorban van az object, de a 0-s virtálus világban - hozzáadjuk a 0s interiort a paraméter sorrend miatt
					if(!object->iWorld)
					{
						strcat(szLine, ", -1"); // Streamer pluginban -1 = minden virtuális világban látható
					}
					sprintf(szHelp, ", %d", object->ucInterior);
					strcat(szLine, szHelp);
				}

				if((flags & HIDE_WHEN_ALPHA_NOT_255) && object->ucAlpha != 0xFF)
				{
					strcat(szLine, "), 0, 0, \"a\", \"a\", 0x000000FF);");
				}
				else
				{
					strcat(szLine, ");");
				}
				strcat(szLine, GetCommect(object->szName, flags));
				fputs(szLine, pfOut);
			}
		}
		fputs("\n/*--------------------------------------------------Remove Objects -------------------------------------------------*/\n", pfOut);
		/////////////////////////////////////////////////
		// Write all remove objects into the file
		{
			for(std::vector<removeobject_t>::iterator object = m_vectorRemoveObjects.begin(); object != m_vectorRemoveObjects.end(); ++object)
			{
				// Call your callback
				if(!(flags & NO_PAWN_CALLS))
					CCallbackManager::OnRemoveObjectDataConverted(*object);

				sprintf(szLine, "RemoveBuildingForPlayer(playerid, %d, %f, %f, %f, %f);\n", object->usModelID, object->vecPos.fX, object->vecPos.fY, object->vecPos.fZ, object->fRadius);
				fputs(szLine, pfOut);
				
			}
		}
		fputs("\n/*--------------------------------------------------Vehicles -------------------------------------------------*/\n", pfOut);
		/////////////////////////////////////////////////
		// Write all vehicles into the file
		{
			for(std::vector<vehicle_t>::iterator vehicle = m_vectorVehicles.begin(); vehicle != m_vectorVehicles.end(); ++vehicle)
			{
				// Call your callback
				if(!(flags & NO_PAWN_CALLS))
					CCallbackManager::OnVehicleDataConverted(*vehicle);
				
				if(flags & ONLY_CREATE_VEHICLE || (!vehicle->ucInterior && !vehicle->iWorld && vehicle->ucPaintjob > 2 && !vehicle->iUpgrades[0]))
				{
					sprintf(szLine, "CreateVehicle(%d, %f, %f, %f, %f, %d, %d, %d);", vehicle->usModelID, vehicle->vecPos.fX, vehicle->vecPos.fY, vehicle->vecPos.fZ,
						vehicle->fAngle, vehicle->iColor1, vehicle->iColor2, m_iRespawnDelay);
					strcat(szLine, GetCommect(vehicle->szName, flags));
					fputs(szLine, pfOut);
				}
				else
				{
					sprintf(szLine, "vid = CreateVehicle(%d, %f, %f, %f, %f, %d, %d, %d);\n", vehicle->usModelID, vehicle->vecPos.fX, vehicle->vecPos.fY, vehicle->vecPos.fZ,
						vehicle->fAngle, vehicle->iColor1, vehicle->iColor2, m_iRespawnDelay);
					fputs(szLine, pfOut);

					if(flags & CONVERT_NUMBER_PLATE)
					{
						sprintf(szLine, "SetVehicleNumberPlate(vid, \"%s\");\n", vehicle->szPlate);
						fputs(szLine, pfOut);
					}

					if(vehicle->ucInterior)
					{
						sprintf(szLine, "LinkVehicleToInterior(vid, %d);\n", vehicle->ucInterior);
						fputs(szLine, pfOut);					
					}
					
					if(vehicle->iWorld)
					{
						sprintf(szLine, "SetVehicleVirtualWorld(vid, %d);\n", vehicle->iWorld);
						fputs(szLine, pfOut);					
					}

					if(vehicle->ucPaintjob < 3)
					{
						sprintf(szLine, "ChangeVehiclePaintjob(vid, %d);\n", vehicle->ucPaintjob);
						fputs(szLine, pfOut);					
					}

					for(uchar i = 0; i != 14; i++)
					{
						if(!vehicle->iUpgrades[i]) continue;
						sprintf(szLine, "AddVehicleComponent(vid, %d);\n", vehicle->iUpgrades[i]);
						fputs(szLine, pfOut);
					}
				}
			}
		}
		fputs("\n/*--------------------------------------------------Checkpoints -------------------------------------------------*/\n", pfOut);
		/////////////////////////////////////////////////
		// Write all markers into the file
		{
			for(std::vector<marker_t>::iterator marker = m_vectorMarkers.begin(); marker != m_vectorMarkers.end(); ++marker)
			{
				// Call your callback
				if(!(flags & NO_PAWN_CALLS))
					CCallbackManager::OnCheckpointDataConverted(*marker);

				// // "arrow", "checkpoint", "corona", "cylinder", "ring"
				switch(marker->szType[1])
				{
					// arrow	
					case 'r':	
					{
						break;
					}
					
					// checkpoint
					case 'h': 
					{
						sprintf(szLine, "CreateDynamicCP(%.4f, %.4f, %.4f, %.4f", marker->vecPos.fX, marker->vecPos.fY, marker->vecPos.fZ, marker->fSize);
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
						sprintf(szLine, "CreateDynamicRaceCP(2, %.4f, %.4f, %.4f, 0.0, 0.0, 0.0, %.4f", marker->vecPos.fX, marker->vecPos.fY, marker->vecPos.fZ, marker->fSize);
						break;
					}

					// ring
					case 'i':
					{
						sprintf(szLine, "CreateDynamicRaceCP(4, %.4f, %.4f, %.4f, 0.0, 0.0, 0.0, %.4f", marker->vecPos.fX, marker->vecPos.fY, marker->vecPos.fZ, marker->fSize);
						break;
					}
				}

				if(marker->iWorld)
				{
					sprintf(szHelp, ", %d", marker->iWorld);
					strcat(szLine, szHelp);
				}

				if(marker->ucInterior)
				{
					if(!marker->iWorld)
						strcat(szLine, ", -1");

					sprintf(szHelp, ", %d", marker->ucInterior);
					strcat(szLine, szHelp);
				}

				strcat(szLine, ");");
				strcat(szLine, GetCommect(marker->szName, flags));
				fputs(szLine, pfOut);
			}
		}
		fputs("\n/*--------------------------------------------------Pickups -------------------------------------------------*/\n", pfOut);
		/////////////////////////////////////////////////
		// Write all pickups into the file
		{
			for(std::vector<pickup_t>::iterator pickup = m_vectorPickups.begin(); pickup != m_vectorPickups.end(); ++pickup)
			{
				// Fix crash
				if(pickup->usModelID > 63) continue;

				// Call your callback
				if(!(flags & NO_PAWN_CALLS))
					CCallbackManager::OnPickupDataConverted(*pickup);

				sprintf(szLine, "CreatePickup(%d, 2, %f, %f, %f", CUtils::GetPickupModelIDFromWeaponID((uchar)pickup->usModelID), pickup->vecPos.fX, pickup->vecPos.fY, pickup->vecPos.fZ);
			
				if(pickup->iWorld)
				{
					sprintf(szHelp, ", %d", pickup->iWorld);
					strcat(szLine, szHelp);
				}

				strcat(szLine, ");");
				strcat(szLine, GetCommect(pickup->szName, flags));
				fputs(szLine, pfOut);
			}
		}	

		//logprintf("Converter stats:");
		CCallbackManager::OnConvertationFinish(m_szWorkingFilename, m_eMapType, flags, m_vectorObjects.size(), m_vectorRemoveObjects.size(), m_vectorVehicles.size(),
			m_vectorMarkers.size(), m_vectorPickups.size());

		logprintf("-> %s.map", ffd.cFileName);
		logprintf("    %d objects converted", m_vectorObjects.size());
		logprintf("    %d remove objects converted", m_vectorRemoveObjects.size());
		logprintf("    %d vehicles converted", m_vectorVehicles.size());
		logprintf("    %d checkpoints converted", m_vectorMarkers.size());
		logprintf("    %d pickups converted\n", m_vectorPickups.size());

		// Clear vectors
		m_vectorObjects.clear();
		m_vectorRemoveObjects.clear();
		m_vectorVehicles.clear();
		m_vectorMarkers.clear();
		m_vectorPickups.clear();
		
		// Close output file
		fclose(pfOut); 
	}
	while (FindNextFile(hFind, &ffd) != 0);
	return true;
}

char *CConverter::GetCommect(char *szFromComment, EConvertingFlags flags)
{
	char szRet[64];
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