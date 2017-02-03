#ifndef CCALLBACKMANAGER_H
#define CCALLBACKMANAGER_H

#include <set>
#include "CConverter.h"
#include "SDK/plugin.h"

class CCallbackManager
{
public:
	static void	RegisterAMX(AMX *pAMX);
	static void	UnregisterAMX(AMX *pAMX);
		
	// Callbacks
	static void OnMapLoadingStart(int mapid, CMap *pMap, bool callPawnFunctions);
	static void OnMapLoadingFinish(int mapid, CMap *pMap, bool callPawnFunctions, int objects, int removeobjects, int vehicles, int checkpoints, int pickups);

	static int	OnObjectDataLoaded(int mapid, object_t *object);
	static void	OnObjectDataUnLoaded(int mapid, int extraid);
	static int	OnRemoveObjectDataLoaded(int mapid, removeobject_t *object);
	static void	OnRemoveObjectDataUnLoaded(int mapid, int extraid);
	static int	OnVehicleDataLoaded(int mapid, vehicle_t *vehicle);
	static void	OnVehicleDataUnLoaded(int mapid, int extraid);
	static int	OnCheckpointDataLoaded(int mapid, marker_t *vehicle);
	static void	OnCheckpointDataUnLoaded(int mapid, int extraid);
	static int	OnPickupDataLoaded(int mapid, pickup_t *vehicle);
	static void	OnPickupDataUnLoaded(int mapid, int extraid);
	
	static std::set<AMX *>		m_listAMX;
};
#endif