#ifndef _CUTILS_H
#define _CUTILS_H

#include "SDK/plugin.h"

class CUtils
{
public:
	static BYTE GetPaletteIndexFromRGB ( BYTE from_r, BYTE from_g, BYTE from_b );
	static float CUtils::RadToDeg(float radians);
	static short GetPickupModelIDFromWeaponID(BYTE weaponid);
	static const char* CUtils::GetWeaponName(BYTE iWeaponID);
};

int set_amxstring(AMX *amx, cell amx_addr, const char *source, int max);
#endif
