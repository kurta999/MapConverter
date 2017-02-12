#pragma once

#include <sdk/plugin.h>

class CUtils
{
public:
	static BYTE GetPaletteIndexFromRGB ( BYTE from_r, BYTE from_g, BYTE from_b );
	static float RadToDeg(float radians);
	static WORD GetWeaponModel(int weaponid);
	const char* GetWeaponName(BYTE iWeaponID);
};

int set_amxstring(AMX *amx, cell amx_addr, const char *source, int max);
bool IsNumeric(const std::string& input);
bool fexists(const std::string& filename);