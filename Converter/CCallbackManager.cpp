#include "stdafx.h"

// OrMisicL - thx
std::list<AMX *> CCallbackManager::m_listAMX;
extern logprintf_t logprintf;
extern CConverter *pConverter;

void CCallbackManager::RegisterAMX(AMX *pAMX)
{
	// Add the amx to the pointers list
	m_listAMX.push_back(pAMX);
}

void CCallbackManager::UnregisterAMX(AMX *pAMX)
{
	// Remove the amx from the pointers list
	m_listAMX.remove(pAMX);
}

void CCallbackManager::OnConvertationStart(char *szFilename, EMapType eMapType, EConvertingFlags flags)
{
	for(std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++)
	{
		// Get the function index
		int iIndex;
		if(!amx_FindPublic((*i), "OnConvertationStart", &iIndex))
		{
			cell addr;

			// Push the parameters
			amx_Push((*i), static_cast<int>(flags));
			amx_Push((*i), static_cast<int>(eMapType));
			amx_PushString(*i, &addr, NULL, pConverter->GetWorkingMapFile(), NULL, NULL);

			// Execute the callback
			amx_Exec((*i), NULL, iIndex);
			amx_Release((*i), addr);
		}
	}
}

void CCallbackManager::OnConvertationFinish(char *szFilename, EMapType eMapType, EConvertingFlags flags, int objects, int removeobjects, int vehicles, int checkpoints, int pickups)
{
	for(std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++)
	{
		// Get the function index
		int iIndex;
		if(!amx_FindPublic((*i), "OnConvertationFinish", &iIndex))
		{
			cell addr;

			// Push the parameters
			amx_Push((*i), pickups);
			amx_Push((*i), checkpoints);
			amx_Push((*i), vehicles);
			amx_Push((*i), removeobjects);
			amx_Push((*i), objects);
			amx_Push((*i), static_cast<int>(flags));
			amx_Push((*i), static_cast<int>(eMapType));
			amx_PushString(*i, &addr, NULL, pConverter->GetWorkingMapFile(), NULL, NULL);

			// Execute the callback
			amx_Exec((*i), NULL, iIndex);
			amx_Release((*i), addr);
		}
	}
}

void CCallbackManager::OnObjectDataConverted(object_t object)
{
	for(std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++)
	{
		// Get the function index
		int iIndex;
		if(!amx_FindPublic((*i), "OnObjectDataConverted", &iIndex))
		{
			// Push the parameters
			amx_Push((*i), object.iWorld);
			amx_Push((*i), (int)object.ucInterior);
			amx_Push((*i), amx_ftoc(object.vecRot.fZ));
			amx_Push((*i), amx_ftoc(object.vecRot.fY));
			amx_Push((*i), amx_ftoc(object.vecRot.fX));
			amx_Push((*i), amx_ftoc(object.vecPos.fZ));
			amx_Push((*i), amx_ftoc(object.vecPos.fY));
			amx_Push((*i), amx_ftoc(object.vecPos.fX));
			amx_Push((*i), (int)object.usModelID);

			// Execute the callback
			amx_Exec((*i), NULL, iIndex);
		}
	}
}

void CCallbackManager::OnRemoveObjectDataConverted(removeobject_t object)
{
	for(std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++)
	{
		// Get the function index
		int iIndex;
		if(!amx_FindPublic((*i), "OnRemoveObjectDataConverted", &iIndex))
		{
			// Push the parameters
			amx_Push((*i), amx_ftoc(object.fRadius));
			amx_Push((*i), amx_ftoc(object.vecPos.fZ));
			amx_Push((*i), amx_ftoc(object.vecPos.fY));
			amx_Push((*i), amx_ftoc(object.vecPos.fX));
			amx_Push((*i), (int)object.usModelID);

			// Execute the callback
			amx_Exec((*i), NULL, iIndex);
		}
	}
}

void CCallbackManager::OnVehicleDataConverted(vehicle_t vehicle)
{
	for(std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++)
	{
		// Get the function index
		int iIndex;
		if(!amx_FindPublic((*i), "OnVehicleDataConverted", &iIndex))
		{
			// Push the parameters
			cell
				ret, 
				addr,
				amx_addr, 
				*phys_addr;
			amx_Allot(*i, sizeof(vehicle.iUpgrades) / sizeof(cell), &amx_addr, &phys_addr);
			memcpy(phys_addr, vehicle.iUpgrades, sizeof(vehicle.iUpgrades));

			amx_Push((*i), vehicle.iWorld);
			amx_Push((*i), (int)vehicle.ucInterior);
			amx_PushString(*i, &addr, NULL, vehicle.szPlate, NULL, NULL);
			amx_Push(*i, amx_addr);
			amx_Push((*i), (int)vehicle.ucPaintjob);
			amx_Push((*i), vehicle.iColor2);
			amx_Push((*i), vehicle.iColor1);
			amx_Push((*i), amx_ftoc(vehicle.fAngle));
			amx_Push((*i), amx_ftoc(vehicle.vecPos.fZ));
			amx_Push((*i), amx_ftoc(vehicle.vecPos.fY));
			amx_Push((*i), amx_ftoc(vehicle.vecPos.fX));
			amx_Push((*i), (int)vehicle.usModelID);

			// Execute the callback
			amx_Exec((*i), NULL, iIndex);
			amx_Release((*i), addr);
		}
	}
}

void CCallbackManager::OnCheckpointDataConverted(marker_t marker)
{
	for(std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++)
	{
		// Get the function index
		int iIndex;
		if(!amx_FindPublic((*i), "OnCheckpointDataConverted", &iIndex))
		{
			// Push the parameters
			cell
				addr;
			amx_Push((*i), marker.iWorld);
			amx_Push((*i), marker.ucInterior);
			amx_Push((*i), amx_ftoc(marker.fSize));
			amx_Push((*i), amx_ftoc(marker.vecPos.fZ));
			amx_Push((*i), amx_ftoc(marker.vecPos.fY));
			amx_Push((*i), amx_ftoc(marker.vecPos.fX));
			amx_PushString(*i, &addr, NULL, marker.szType, NULL, NULL);

			// Execute the callback
			amx_Exec((*i), NULL, iIndex);
			amx_Release((*i), addr);
		}
	}
}

void CCallbackManager::OnPickupDataConverted(pickup_t picup)
{
	for(std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++)
	{
		// Get the function index
		int iIndex;
		if(!amx_FindPublic((*i), "OnPickupDataConverted", &iIndex))
		{
			// Push the parameters
			amx_Push((*i), picup.iWorld);
			amx_Push((*i), amx_ftoc(picup.vecPos.fZ));
			amx_Push((*i), amx_ftoc(picup.vecPos.fY));
			amx_Push((*i), amx_ftoc(picup.vecPos.fX));
			amx_Push((*i), picup.usModelID);

			// Execute the callback
			amx_Exec((*i), NULL, iIndex);
		}
	}
}