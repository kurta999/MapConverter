#ifndef _CUTILS_H
#define _CUTILS_H

class CUtils
{
public:
	static uchar GetPaletteIndexFromRGB ( uchar from_r, uchar from_g, uchar from_b );
	static float CUtils::RadToDeg(float radians);
	static short GetPickupModelIDFromWeaponID(uchar weaponid);
	static const char* CUtils::GetWeaponName(uchar iWeaponID);
};

#endif
