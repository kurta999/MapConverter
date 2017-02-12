#pragma once

#include "CVector.h"
#include "CSingleton.h"

#include <stdio.h>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#ifndef _WIN32
typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;
#endif

struct object_t
{
	object_t() : extraID(0) { }

	WORD wModelID;
	CVector vecPos;
	CVector vecRot;
	BYTE byteInterior;
	int iWorld;
	BYTE ucAlpha;
	std::string strName;
	int extraID;
};

struct removeobject_t
{
	removeobject_t() : extraID(0) { }

	WORD wModelID;
	CVector vecPos;
	float fRadius;
	int extraID;
};

struct vehicle_t
{
	vehicle_t() : extraID(0) { }

	WORD wModelID;
	CVector vecPos;
	float fAngle;
	int iColor1, iColor2;
	BYTE ucPaintjob;
	int iUpgrades[14];
	char szPlate[32];
	BYTE byteInterior;
	int iWorld;
	std::string strName;
	int extraID;
};

struct marker_t
{
	marker_t() : extraID(0) { }

	enum class MarkerType : BYTE
	{
		ARROW,
		CHECKPOINT,
		CORONA,
		CYLINDER,
		RING
	};

	MarkerType type;
	CVector vecPos;
	float fSize;
	DWORD dwColor;
	BYTE byteInterior;
	int iWorld;
	std::string strName;
	int extraID;
};

struct pickup_t
{
	pickup_t() : extraID(0), byteInterior(0) { }

	WORD wModelID;
	CVector vecPos;
	BYTE byteInterior;
	int iWorld;
	std::string strName;
	int extraID;
};

struct ped_t
{
	ped_t() : extraID(0), byteInterior(0) { }

	WORD wModelID;
	CVector vecPos;
	float fAngle;
	BYTE byteInterior;
	int iWorld;
	std::string strName;
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
	CMap(std::string const &name)
	{
		mapName = name;
	}
	
	CMap(std::string const &name, EMapType maptype) :
		mapType(mapType)
	{
		mapName = name;
	}

	inline void Insert(object_t *object)
	{
		vectorObjects.push_back(object);
	}

	inline void Insert(removeobject_t *object)
	{		
		vectorRemoveObjects.push_back(object);
	}

	inline void Insert(vehicle_t *object)
	{		
		vectorVehicles.push_back(object);
	}

	inline void Insert(marker_t *object)
	{		
		vectorMarkers.push_back(object);
	}

	inline void Insert(pickup_t *object)
	{		
		vectorPickups.push_back(object);
	}

	inline void Insert(ped_t *object)
	{
		vectorPeds.push_back(object);
	}

	std::string mapName;
	EMapType mapType;
	std::vector<object_t*> vectorObjects;
	std::vector<removeobject_t*> vectorRemoveObjects;
	std::vector<vehicle_t*> vectorVehicles;
	std::vector<marker_t*> vectorMarkers;
	std::vector<pickup_t*> vectorPickups;
	std::vector<ped_t*> vectorPeds;
};

class CConverter : public CSingleton<CConverter>
{
	friend class CSingleton<CConverter>;
public:
	CConverter();
	~CConverter();
	
	int LoadMap(std::string const &strPath, bool callPawnFunctions);
	bool IsValidMTAMap(int mapID);
	bool UnLoadMTAMap(int mapID, bool callPawnFunctions);
	bool SaveMTAMap(int mapID, ESavingFlags flags);
	std::string *GetMTAMapName(int mapID);
	int GetMapIDFromName(std::string const &strMap);

private:
	int LoadMapMTA(std::string const &strPath, bool callPawnFunctions);
	int LoadMapIPL(std::string const &strPath, bool callPawnFunctions);
	const char *GetCommect(std::string const &szFromComment, ESavingFlags flags);

	int mapUpperID;
	std::unordered_map<int, CMap*> maps;
	std::unordered_map<std::string, int> mapNames;
};