#ifndef CCONVERTER_H
#define CCONVERTER_H

#include <stdio.h>
#include <vector>
#include "CVector.h"
#include <unordered_map>
#include <unordered_set>

struct object_t
{
	object_t::object_t() : extraID(0) { }

	WORD usModelID;
	CVector vecPos;
	CVector vecRot;
	BYTE ucInterior;
	int iWorld;
	BYTE ucAlpha;
	char szName[32];
	int extraID;
};

struct removeobject_t
{
	removeobject_t::removeobject_t() : extraID(0) { }

	WORD usModelID;
	CVector vecPos;
	float fRadius;
	int extraID;
};

struct vehicle_t
{
	vehicle_t::vehicle_t() : extraID(0) { }

	WORD usModelID;
	CVector vecPos;
	float fAngle;
	int iColor1, iColor2;
	BYTE ucPaintjob;
	int iUpgrades[14];
	char szPlate[32];
	BYTE ucInterior;
	int iWorld;
	char szName[32];
	int extraID;
};

struct marker_t
{
	marker_t::marker_t() : extraID(0) { }

	char szType[24];
	CVector vecPos;
	float fSize;
	BYTE ucInterior;
	int iWorld;
	char szName[32];
	int extraID;
};

struct pickup_t
{
	pickup_t::pickup_t() : extraID(0) { }

	WORD usModelID;
	CVector vecPos;
	int iWorld;
	char szName[32];
	int extraID;
};

enum EMapType : BYTE
{
	MTA_RACE = 0,
	MTA_DM = 1,
	GTA_IPL = 2,
};

enum ESavingFlags : BYTE
{
	ONLY_CREATE_VEHICLE = 1,
	SAVE_NUMBER_PLATE = 2,
	HIDE_WHEN_ALPHA_NOT_255 = 4,
	CONVERT_ELEMENT_NAME = 8,
};

class CMap
{
public:
	CMap::CMap(std::string name)
	{
		mapName = name;
	}
	
	inline void CMap::Insert(object_t *object)
	{
		vectorObjects.push_back(object);
	}

	inline void CMap::Insert(removeobject_t *object)
	{		
		vectorRemoveObjects.push_back(object);
	}

	inline void CMap::Insert(vehicle_t *object)
	{		
		vectorVehicles.push_back(object);
	}

	inline void CMap::Insert(marker_t *object)
	{		
		vectorMarkers.push_back(object);
	}

	inline void CMap::Insert(pickup_t *object)
	{		
		vectorPickups.push_back(object);
	}

	std::string mapName;
	EMapType mapType;
	std::vector<object_t*> vectorObjects;
	std::vector<removeobject_t*> vectorRemoveObjects;
	std::vector<vehicle_t*> vectorVehicles;
	std::vector<marker_t*> vectorMarkers;
	std::vector<pickup_t*> vectorPickups;
};

class CConverter 
{
public:
	CConverter();
	~CConverter();
	
	int LoadAllMTAMap(bool callPawnFunctions);
	int LoadMTAMap(std::string strPath, bool callPawnFunctions);
	int LoadIPL(std::string strPath, bool callPawnFunctions);
	bool IsValidMTAMap(int mapID);
	bool UnLoadMTAMap(int mapID, bool callPawnFunctions);
	bool SaveMTAMap(int mapID, ESavingFlags flags);
	std::string *GetMTAMapName(int mapID);
	int GetMapIDFromName(std::string strMap);

private:
	void ConvertMTAMap(char);
	char *GetCommect(char *szFromComment, ESavingFlags flags);

	int mapUpperID;
	std::unordered_map<int, CMap*> maps;
	std::unordered_map<std::string, int> mapNames;
};

#endif
