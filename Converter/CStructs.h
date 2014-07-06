#ifndef CSTRUCTS_H
#define CSTRUCTS_H

#include "CVector.h"

typedef unsigned short ushort;
typedef unsigned char uchar;

enum EMapType : int
{
	MTA_RACE = 0,
	MTA_DM = 1,
};

struct object_t
{
	ushort usModelID;
	CVector vecPos;
	CVector vecRot;
	uchar ucInterior;
	int iWorld;
	uchar ucAlpha;
	char szName[32];
};

struct removeobject_t
{
	ushort usModelID;
	CVector vecPos;
	float fRadius;
};

struct vehicle_t
{
	ushort usModelID;
	CVector vecPos;
	float fAngle;
	int iColor1, iColor2;
	uchar ucPaintjob;
	int iUpgrades[14];
	char szPlate[32];
	uchar ucInterior;
	int iWorld;
	char szName[32];
};

struct marker_t
{
	char szType[24];
	CVector vecPos;
	float fSize;
	uchar ucInterior;
	int iWorld;
	char szName[32];
};

struct pickup_t
{
	ushort usModelID;
	CVector vecPos;
	int iWorld;
	char szName[32];
};
#endif