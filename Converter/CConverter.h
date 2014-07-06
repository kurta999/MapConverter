#ifndef CCONVERTER_H
#define CCONVERTER_H

#include <stdio.h>
#include <vector>
#include "CVector.h"
#include "CStructs.h"

enum EConvertingFlags
{
	ONLY_CREATE_VEHICLE = 1,
	CONVERT_NUMBER_PLATE = 2,
	HIDE_WHEN_ALPHA_NOT_255 = 4,
	CONVERT_ELEMENT_NAME = 8,
	NO_PAWN_CALLS = 16,
};

class CConverter
{
public:
	CConverter();
	~CConverter();
	bool ConvertMTAMapToSAMP(char *szPath, EConvertingFlags flags);

	void SetVehiclesRespawnDelay(int delay) { m_iRespawnDelay = delay; }
	int GetVehiclesRespawnDelay(void)	{ return m_iRespawnDelay; }

	char *GetWorkingMapFile(void) { return m_szWorkingFilename; }
	EMapType GetMapType(void) { return m_eMapType; }
private:
	char *GetCommect(char *szFromComment, EConvertingFlags flags);

	std::vector<object_t> m_vectorObjects;
	std::vector<removeobject_t> m_vectorRemoveObjects;
	std::vector<vehicle_t> m_vectorVehicles;
	std::vector<marker_t> m_vectorMarkers;
	std::vector<pickup_t> m_vectorPickups;

	int m_iRespawnDelay;
	char *m_szWorkingFilename;
	EMapType m_eMapType;
};

#endif
