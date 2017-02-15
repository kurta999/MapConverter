#pragma once

#include <sdk/plugin.h>

typedef struct quat_s
{
	double x;
	double y;
	double z;
	double w;
} quat_t;

class CUtils
{
public:
	static BYTE GetPaletteIndexFromRGB ( BYTE from_r, BYTE from_g, BYTE from_b );
	static float RadToDeg(float radians);
	static void QuatToEuler(const quat_t& rotation, CVector& result);
	static WORD GetWeaponModel(int weaponid);
	const char* GetWeaponName(BYTE iWeaponID);
};

int set_amxstring(AMX *amx, cell amx_addr, const char *source, int max);
bool IsNumeric(const std::string& input);
bool fexists(const std::string& filename);