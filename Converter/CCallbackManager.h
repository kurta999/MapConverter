#ifndef CCALLBACKMANAGER_H
#define CCALLBACKMANAGER_H

#include <list>
#include "CVector.h"
#include "CStructs.h"
#include "SDK/plugin.h"

enum EConvertingFlags;
class CCallbackManager
{
public:
	static void	RegisterAMX(AMX *pAMX);
	static void	UnregisterAMX(AMX *pAMX);
		
	// Callbacks
	static void OnConvertationStart(char *szFilename, EMapType eMapType, EConvertingFlags flags);
	static void OnConvertationFinish(char *szFilename, EMapType eMapType, EConvertingFlags flags, int objects, int removeobjects, int vehicles, int checkpoints, int pickups);

	static void	OnObjectDataConverted(object_t object);
	static void	OnRemoveObjectDataConverted(removeobject_t object);
	static void	OnVehicleDataConverted(vehicle_t vehicle);
	static void	OnCheckpointDataConverted(marker_t vehicle);
	static void	OnPickupDataConverted(pickup_t vehicle);
	
	static std::list<AMX *>		m_listAMX;
};
#endif