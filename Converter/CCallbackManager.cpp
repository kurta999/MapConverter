#include "main.h"

// OrMisicL - thx
std::list<AMX *> CCallbackManager::m_listAMX;

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

void CCallbackManager::OnMapLoadingStart(int mapid, CMap *pMap, bool callPawnFunctions)
{
	for(std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++)
	{
		// Get the function index
		int iIndex;
		if(!amx_FindPublic((*i), "OnMapLoadingStart", &iIndex))
		{
			cell addr;

			// Push the parameters
			amx_Push((*i), static_cast<cell>(callPawnFunctions));
			amx_Push((*i), static_cast<cell>(pMap->mapType));
			amx_PushString(*i, &addr, NULL, pMap->mapName.c_str(), NULL, NULL);
			amx_Push((*i), static_cast<cell>(mapid));

			// Execute the callback
			amx_Exec((*i), NULL, iIndex);
			amx_Release((*i), addr);
		}
	}
}

void CCallbackManager::OnMapLoadingFinish(int mapid, CMap *pMap, bool callPawnFunctions, int objects, int removeobjects, int vehicles, int checkpoints, int pickups)
{
	for(std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++)
	{
		// Get the function index
		int iIndex;
		if(!amx_FindPublic((*i), "OnMapLoadingFinish", &iIndex))
		{
			cell addr;

			// Push the parameters
			amx_Push((*i), static_cast<cell>(pickups));
			amx_Push((*i), static_cast<cell>(checkpoints));
			amx_Push((*i), static_cast<cell>(vehicles));
			amx_Push((*i), static_cast<cell>(removeobjects));
			amx_Push((*i), static_cast<cell>(objects));
			amx_Push((*i), static_cast<cell>(callPawnFunctions));
			amx_Push((*i), static_cast<cell>(pMap->mapType));
			amx_PushString(*i, &addr, NULL, pMap->mapName.c_str(), NULL, NULL);
			amx_Push((*i), static_cast<cell>(mapid));

			// Execute the callback
			amx_Exec((*i), NULL, iIndex);
			amx_Release((*i), addr);
		}
	}
}

int CCallbackManager::OnObjectDataLoaded(int mapid, object_t *object)
{
	cell ret = -1;
	for(std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++)
	{
		// Get the function index
		int iIndex;
		if(!amx_FindPublic((*i), "OnObjectDataLoaded", &iIndex))
		{
			// Push the parameters
			amx_Push((*i), static_cast<cell>(object->iWorld));
			amx_Push((*i), static_cast<cell>(object->ucInterior));
			amx_Push((*i), amx_ftoc(object->vecRot.fZ));
			amx_Push((*i), amx_ftoc(object->vecRot.fY));
			amx_Push((*i), amx_ftoc(object->vecRot.fX));
			amx_Push((*i), amx_ftoc(object->vecPos.fZ));
			amx_Push((*i), amx_ftoc(object->vecPos.fY));
			amx_Push((*i), amx_ftoc(object->vecPos.fX));
			amx_Push((*i), static_cast<cell>(object->usModelID));
			amx_Push((*i), static_cast<cell>(mapid));

			// Execute the callback
			amx_Exec((*i), &ret, iIndex);
		}
	}
	return static_cast<int>(ret);
}

void CCallbackManager::OnObjectDataUnLoaded(int mapid, int extraid)
{
	cell ret = -1;
	for(std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++)
	{
		// Get the function index
		int iIndex;
		if(!amx_FindPublic((*i), "OnObjectDataUnLoaded", &iIndex))
		{
			// Push the parameters
			amx_Push((*i), static_cast<cell>(extraid));
			amx_Push((*i), static_cast<cell>(mapid));

			// Execute the callback
			amx_Exec((*i), &ret, iIndex);
		}
	}
}

int CCallbackManager::OnRemoveObjectDataLoaded(int mapid, removeobject_t *object)
{
	cell ret = -1;
	for(std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++)
	{
		// Get the function index
		int iIndex;
		if(!amx_FindPublic((*i), "OnRemoveObjectDataLoaded", &iIndex))
		{
			// Push the parameters
			amx_Push((*i), amx_ftoc(object->fRadius));
			amx_Push((*i), amx_ftoc(object->vecPos.fZ));
			amx_Push((*i), amx_ftoc(object->vecPos.fY));
			amx_Push((*i), amx_ftoc(object->vecPos.fX));
			amx_Push((*i), static_cast<cell>(object->usModelID));
			amx_Push((*i), static_cast<cell>(mapid));

			// Execute the callback
			amx_Exec((*i), &ret, iIndex);
		}
	}
	return static_cast<int>(ret);
}

void CCallbackManager::OnRemoveObjectDataUnLoaded(int mapid, int extraid)
{
	cell ret = -1;
	for (std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++)
	{
		// Get the function index
		int iIndex;
		if (!amx_FindPublic((*i), "OnRemoveObjectDataUnLoaded", &iIndex))
		{
			// Push the parameters
			amx_Push((*i), static_cast<cell>(extraid));
			amx_Push((*i), static_cast<cell>(mapid));

			// Execute the callback
			amx_Exec((*i), &ret, iIndex);
		}
	}
}

int CCallbackManager::OnVehicleDataLoaded(int mapid, vehicle_t *vehicle)
{
	cell ret = -1;
	for(std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++)
	{
		// Get the function index
		int iIndex;
		if(!amx_FindPublic((*i), "OnVehicleDataLoaded", &iIndex))
		{
			// Push the parameters
			cell
				addr,
				amx_addr, 
				*phys_addr;
			amx_Allot(*i, sizeof(vehicle->iUpgrades) / sizeof(cell), &amx_addr, &phys_addr);
			memcpy(phys_addr, vehicle->iUpgrades, sizeof(vehicle->iUpgrades));

			amx_Push((*i), static_cast<cell>(vehicle->iWorld));
			amx_Push((*i), static_cast<cell>(vehicle->ucInterior));
			amx_PushString(*i, &addr, NULL, vehicle->szPlate, NULL, NULL);
			amx_Push(*i, amx_addr);
			amx_Push((*i), static_cast<cell>(vehicle->ucPaintjob));
			amx_Push((*i), static_cast<cell>(vehicle->iColor2));
			amx_Push((*i), static_cast<cell>(vehicle->iColor1));
			amx_Push((*i), amx_ftoc(vehicle->fAngle));
			amx_Push((*i), amx_ftoc(vehicle->vecPos.fZ));
			amx_Push((*i), amx_ftoc(vehicle->vecPos.fY));
			amx_Push((*i), amx_ftoc(vehicle->vecPos.fX));
			amx_Push((*i), static_cast<cell>(vehicle->usModelID));
			amx_Push((*i), static_cast<cell>(vehicle->usModelID));
			amx_Push((*i), static_cast<cell>(mapid));

			// Execute the callback
			amx_Exec((*i), &ret, iIndex);
			amx_Release((*i), addr);
		}
	}
	return static_cast<int>(ret);
}

void CCallbackManager::OnVehicleDataUnLoaded(int mapid, int extraid)
{
	cell ret = -1;
	for (std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++)
	{
		// Get the function index
		int iIndex;
		if (!amx_FindPublic((*i), "OnVehicleDataUnLoaded", &iIndex))
		{
			// Push the parameters
			amx_Push((*i), static_cast<cell>(extraid));
			amx_Push((*i), static_cast<cell>(mapid));

			// Execute the callback
			amx_Exec((*i), &ret, iIndex);
		}
	}
}

int CCallbackManager::OnCheckpointDataLoaded(int mapid, marker_t *marker)
{
	cell ret = -1;
	for(std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++)
	{
		// Get the function index
		int iIndex;
		if(!amx_FindPublic((*i), "OnCheckpointDataLoaded", &iIndex))
		{
			// Push the parameters
			cell
				addr;
			amx_Push((*i), static_cast<cell>(marker->iWorld));
			amx_Push((*i), static_cast<cell>(marker->ucInterior));
			amx_Push((*i), amx_ftoc(marker->fSize));
			amx_Push((*i), amx_ftoc(marker->vecPos.fZ));
			amx_Push((*i), amx_ftoc(marker->vecPos.fY));
			amx_Push((*i), amx_ftoc(marker->vecPos.fX));
			amx_PushString(*i, &addr, NULL, marker->szType, NULL, NULL);
			amx_Push((*i), static_cast<cell>(mapid));

			// Execute the callback
			amx_Exec((*i), &ret, iIndex);
			amx_Release((*i), addr);
		}
	}
	return static_cast<int>(ret);
}

void CCallbackManager::OnCheckpointDataUnLoaded(int mapid, int extraid)
{
	cell ret = -1;
	for (std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++)
	{
		// Get the function index
		int iIndex;
		if (!amx_FindPublic((*i), "OnCheckpointDataUnLoaded", &iIndex))
		{
			// Push the parameters
			amx_Push((*i), static_cast<cell>(extraid));
			amx_Push((*i), static_cast<cell>(mapid));

			// Execute the callback
			amx_Exec((*i), &ret, iIndex);
		}
	}
}

int CCallbackManager::OnPickupDataLoaded(int mapid, pickup_t *picup)
{
	cell ret = -1;
	for(std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++)
	{
		// Get the function index
		int iIndex;
		if(!amx_FindPublic((*i), "OnPickupDataLoaded", &iIndex))
		{
			// Push the parameters
			amx_Push((*i), static_cast<cell>(picup->iWorld));
			amx_Push((*i), amx_ftoc(picup->vecPos.fZ));
			amx_Push((*i), amx_ftoc(picup->vecPos.fY));
			amx_Push((*i), amx_ftoc(picup->vecPos.fX));
			amx_Push((*i), static_cast<cell>(picup->usModelID));

			// Execute the callback
			amx_Exec((*i), &ret, iIndex);
			break;
		}
	}
	return static_cast<int>(ret);
}

void CCallbackManager::OnPickupDataUnLoaded(int mapid, int extraid)
{
	cell ret = -1;
	for (std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++)
	{
		// Get the function index
		int iIndex;
		if (!amx_FindPublic((*i), "OnPickupDataUnLoaded", &iIndex))
		{
			// Push the parameters
			amx_Push((*i), static_cast<cell>(extraid));
			amx_Push((*i), static_cast<cell>(mapid));

			// Execute the callback
			amx_Exec((*i), &ret, iIndex);
		}
	}
}