#include "main.h"
#include <algorithm>


// https://github.com/multitheftauto/mtasa-blue/blob/55c5a42ee29a79554a3a09f3d19da5ced20ca742/Shared/sdk/SharedUtil.Game.hpp#L192
static const BYTE paletteColorTable8[] = {
            0x00, 0x00, 0x00, 0xff, 0xf5, 0xf5, 0xf5, 0xff, 0x2a, 0x77, 0xa1, 0xff, 0x84, 0x04, 0x10, 0xff,
            0x26, 0x37, 0x39, 0xff, 0x86, 0x44, 0x6e, 0xff, 0xd7, 0x8e, 0x10, 0xff, 0x4c, 0x75, 0xb7, 0xff,
            0xbd, 0xbe, 0xc6, 0xff, 0x5e, 0x70, 0x72, 0xff, 0x46, 0x59, 0x7a, 0xff, 0x65, 0x6a, 0x79, 0xff,
            0x5d, 0x7e, 0x8d, 0xff, 0x58, 0x59, 0x5a, 0xff, 0xd6, 0xda, 0xd6, 0xff, 0x9c, 0xa1, 0xa3, 0xff,
            0x33, 0x5f, 0x3f, 0xff, 0x73, 0x0e, 0x1a, 0xff, 0x7b, 0x0a, 0x2a, 0xff, 0x9f, 0x9d, 0x94, 0xff,
            0x3b, 0x4e, 0x78, 0xff, 0x73, 0x2e, 0x3e, 0xff, 0x69, 0x1e, 0x3b, 0xff, 0x96, 0x91, 0x8c, 0xff,
            0x51, 0x54, 0x59, 0xff, 0x3f, 0x3e, 0x45, 0xff, 0xa5, 0xa9, 0xa7, 0xff, 0x63, 0x5c, 0x5a, 0xff,
            0x3d, 0x4a, 0x68, 0xff, 0x97, 0x95, 0x92, 0xff, 0x42, 0x1f, 0x21, 0xff, 0x5f, 0x27, 0x2b, 0xff,
            0x84, 0x94, 0xab, 0xff, 0x76, 0x7b, 0x7c, 0xff, 0x64, 0x64, 0x64, 0xff, 0x5a, 0x57, 0x52, 0xff,
            0x25, 0x25, 0x27, 0xff, 0x2d, 0x3a, 0x35, 0xff, 0x93, 0xa3, 0x96, 0xff, 0x6d, 0x7a, 0x88, 0xff,
            0x22, 0x19, 0x18, 0xff, 0x6f, 0x67, 0x5f, 0xff, 0x7c, 0x1c, 0x2a, 0xff, 0x5f, 0x0a, 0x15, 0xff,
            0x19, 0x38, 0x26, 0xff, 0x5d, 0x1b, 0x20, 0xff, 0x9d, 0x98, 0x72, 0xff, 0x7a, 0x75, 0x60, 0xff,
            0x98, 0x95, 0x86, 0xff, 0xad, 0xb0, 0xb0, 0xff, 0x84, 0x89, 0x88, 0xff, 0x30, 0x4f, 0x45, 0xff,
            0x4d, 0x62, 0x68, 0xff, 0x16, 0x22, 0x48, 0xff, 0x27, 0x2f, 0x4b, 0xff, 0x7d, 0x62, 0x56, 0xff,
            0x9e, 0xa4, 0xab, 0xff, 0x9c, 0x8d, 0x71, 0xff, 0x6d, 0x18, 0x22, 0xff, 0x4e, 0x68, 0x81, 0xff,
            0x9c, 0x9c, 0x98, 0xff, 0x91, 0x73, 0x47, 0xff, 0x66, 0x1c, 0x26, 0xff, 0x94, 0x9d, 0x9f, 0xff,
            0xa4, 0xa7, 0xa5, 0xff, 0x8e, 0x8c, 0x46, 0xff, 0x34, 0x1a, 0x1e, 0xff, 0x6a, 0x7a, 0x8c, 0xff,
            0xaa, 0xad, 0x8e, 0xff, 0xab, 0x98, 0x8f, 0xff, 0x85, 0x1f, 0x2e, 0xff, 0x6f, 0x82, 0x97, 0xff,
            0x58, 0x58, 0x53, 0xff, 0x9a, 0xa7, 0x90, 0xff, 0x60, 0x1a, 0x23, 0xff, 0x20, 0x20, 0x2c, 0xff,
            0xa4, 0xa0, 0x96, 0xff, 0xaa, 0x9d, 0x84, 0xff, 0x78, 0x22, 0x2b, 0xff, 0x0e, 0x31, 0x6d, 0xff,
            0x72, 0x2a, 0x3f, 0xff, 0x7b, 0x71, 0x5e, 0xff, 0x74, 0x1d, 0x28, 0xff, 0x1e, 0x2e, 0x32, 0xff,
            0x4d, 0x32, 0x2f, 0xff, 0x7c, 0x1b, 0x44, 0xff, 0x2e, 0x5b, 0x20, 0xff, 0x39, 0x5a, 0x83, 0xff,
            0x6d, 0x28, 0x37, 0xff, 0xa7, 0xa2, 0x8f, 0xff, 0xaf, 0xb1, 0xb1, 0xff, 0x36, 0x41, 0x55, 0xff,
            0x6d, 0x6c, 0x6e, 0xff, 0x0f, 0x6a, 0x89, 0xff, 0x20, 0x4b, 0x6b, 0xff, 0x2b, 0x3e, 0x57, 0xff,
            0x9b, 0x9f, 0x9d, 0xff, 0x6c, 0x84, 0x95, 0xff, 0x4d, 0x84, 0x95, 0xff, 0xae, 0x9b, 0x7f, 0xff,
            0x40, 0x6c, 0x8f, 0xff, 0x1f, 0x25, 0x3b, 0xff, 0xab, 0x92, 0x76, 0xff, 0x13, 0x45, 0x73, 0xff,
            0x96, 0x81, 0x6c, 0xff, 0x64, 0x68, 0x6a, 0xff, 0x10, 0x50, 0x82, 0xff, 0xa1, 0x99, 0x83, 0xff,
            0x38, 0x56, 0x94, 0xff, 0x52, 0x56, 0x61, 0xff, 0x7f, 0x69, 0x56, 0xff, 0x8c, 0x92, 0x9a, 0xff,
            0x59, 0x6e, 0x87, 0xff, 0x47, 0x35, 0x32, 0xff, 0x44, 0x62, 0x4f, 0xff, 0x73, 0x0a, 0x27, 0xff,
            0x22, 0x34, 0x57, 0xff, 0x64, 0x0d, 0x1b, 0xff, 0xa3, 0xad, 0xc6, 0xff, 0x69, 0x58, 0x53, 0xff,
            0x9b, 0x8b, 0x80, 0xff, 0x62, 0x0b, 0x1c, 0xff, 0x5b, 0x5d, 0x5e, 0xff, 0x62, 0x44, 0x28, 0xff,
            0x73, 0x18, 0x27, 0xff, 0x1b, 0x37, 0x6d, 0xff, 0xec, 0x6a, 0xae, 0xff,
        };

// https://github.com/multitheftauto/mtasa-blue/blob/dd4d47a2efa93ba6030b9f8620de0faacb901d2b/Client/mods/deathmatch/logic/CClientPickupManager.cpp#L17
static const WORD g_usWeaponModels[] =
{
	0, 331, 333, 334, 335, 336, 337, 338, 339, 341,     // 9
	321, 322, 323, 0, 325, 326, 342, 343, 344, 0,       // 19
	0, 0, 346, 347, 348, 349, 350, 351, 352, 353,       // 29
	355, 356, 372, 357, 358, 359, 360, 361, 362, 363,   // 39
	364, 365, 366, 367, 368, 369, 371                   // 46
};

#define WEAPON_BRASSKNUCKLE				1
#define WEAPON_GOLFCLUB					2
#define WEAPON_NITESTICK				3
#define WEAPON_KNIFE					4
#define WEAPON_BAT						5
#define WEAPON_SHOVEL					6
#define WEAPON_POOLSTICK				7
#define WEAPON_KATANA					8
#define WEAPON_CHAINSAW					9
#define WEAPON_DILDO					10
#define WEAPON_DILDO2					11
#define WEAPON_VIBRATOR					12
#define WEAPON_VIBRATOR2				13
#define WEAPON_FLOWER					14
#define WEAPON_CANE						15
#define WEAPON_GRENADE					16
#define WEAPON_TEARGAS					17
#define WEAPON_MOLTOV					18
#define WEAPON_COLT45					22
#define WEAPON_SILENCED					23
#define WEAPON_DEAGLE					24
#define WEAPON_SHOTGUN					25
#define WEAPON_SAWEDOFF					26
#define WEAPON_SHOTGSPA					27
#define WEAPON_UZI						28
#define WEAPON_MP5						29
#define WEAPON_AK47						30
#define WEAPON_M4						31
#define WEAPON_TEC9						32
#define WEAPON_RIFLE					33
#define WEAPON_SNIPER					34
#define WEAPON_ROCKETLAUNCHER			35
#define WEAPON_HEATSEEKER				36
#define WEAPON_FLAMETHROWER				37
#define WEAPON_MINIGUN					38
#define WEAPON_SATCHEL					39
#define WEAPON_BOMB						40
#define WEAPON_SPRAYCAN					41
#define WEAPON_FIREEXTINGUISHER			42
#define WEAPON_CAMERA (43)
#define WEAPON_NIGHTVISION (44)
#define WEAPON_INFRARED (45)
#define WEAPON_PARACHUTE (46)
#define WEAPON_VEHICLE (49)
#define WEAPON_DROWN (53)
#define WEAPON_COLLISION (54)

// https://github.com/multitheftauto/mtasa-blue/blob/55c5a42ee29a79554a3a09f3d19da5ced20ca742/Shared/sdk/SharedUtil.Game.hpp#L228
BYTE CUtils::GetPaletteIndexFromRGB ( BYTE from_r, BYTE from_g, BYTE from_b )
{
    unsigned long ulBestDist = 0xFFFFFFFF;
    BYTE ucBestMatch = 0;
    for ( unsigned int i = 0 ; i < NUMELMS( paletteColorTable8 ) / 4 ; i++ )
    {
        int r = paletteColorTable8[ i * 4 + 0 ] - from_r;
        int g = paletteColorTable8[ i * 4 + 1 ] - from_g;
        int b = paletteColorTable8[ i * 4 + 2 ] - from_b;
        unsigned long ulDist = r * r + g * g + b * b;
        if ( ulDist < ulBestDist )
        {
            ulBestDist = ulDist;
            ucBestMatch = i;
        }
    }
    return ucBestMatch;
}

// Y_Less - y_loader - thx
float CUtils::RadToDeg(float radians)
{
	if (radians == 0.0)
	{
		return 0.0;
	}
	float retval = (360.0f + (radians / 0.0174532925f));
	while (retval >= 360.0f) retval -= 360.0f;
	while (retval < 0.0f) retval += 360.0f;
	return retval;
}

// https://github.com/Pottus/ColAndreas/blob/28775f0f39a25a84349473b206118fa4b0fd944a/src/DynamicWorld.cpp#L63
const double RADIAN_TO_DEG = 57.29577951;

void CUtils::QuatToEuler(const quat_t& rotation, CVector& result)
{
	result.fY = (float)((-asin(2 * ((rotation.x * rotation.z) + (rotation.w * rotation.y))) * RADIAN_TO_DEG));
	result.fX = (float)((atan2(2 * ((rotation.y * rotation.z) + (rotation.w * rotation.x)), (rotation.w * rotation.w) - (rotation.x * rotation.x) - (rotation.y * rotation.y) + (rotation.z * rotation.z)) * RADIAN_TO_DEG));
	result.fZ = (float)((-atan2(2 * ((rotation.x * rotation.y) + (rotation.w * rotation.z)), (rotation.w * rotation.w) + (rotation.x * rotation.x) - (rotation.y * rotation.y) - (rotation.z * rotation.z)) * RADIAN_TO_DEG));
}

WORD CUtils::GetWeaponModel(int weaponid)
{
	if(weaponid < 0 || weaponid > 46) return 0;

	return g_usWeaponModels[weaponid];
}

const char* CUtils::GetWeaponName(BYTE iWeaponID)
{
	switch (iWeaponID)
	{
		case 0:
			return "Fists";
		case WEAPON_BRASSKNUCKLE:
			return "Brass Knuckles";
		case WEAPON_GOLFCLUB:
			return "Golf Club";
		case WEAPON_NITESTICK:
			return "Nite Stick";
		case WEAPON_KNIFE:
			return "Knife";
		case WEAPON_BAT:
			return "Baseball Bat";
		case WEAPON_SHOVEL:
			return "Shovel";
		case WEAPON_POOLSTICK:
			return "Pool Cue";
		case WEAPON_KATANA:
			return "Katana";
		case WEAPON_CHAINSAW:
			return "Chainsaw";
		case WEAPON_DILDO:
			return "Dildo";
		case WEAPON_DILDO2:
			return "Dildo";
		case WEAPON_VIBRATOR:
			return "Vibrator";
		case WEAPON_VIBRATOR2:
			return "Vibrator";
		case WEAPON_FLOWER:
			return "Flowers";
		case WEAPON_CANE:
			return "Cane";
		case WEAPON_GRENADE:
			return "Grenade";
		case WEAPON_TEARGAS:
			return "Teargas";
		case WEAPON_MOLTOV:
			return "Molotov";
		case 19:
			return "Vehicle Missile";
		case 20:
			return "Hydra Flare";
		case 21:
			return "Jetpack";
		case WEAPON_COLT45:
			return "Colt 45";
		case WEAPON_SILENCED:
			return "Silenced Pistol";
		case WEAPON_DEAGLE:
			return "Desert Eagle";
		case WEAPON_SHOTGUN:
			return "Shotgun";
		case WEAPON_SAWEDOFF:
			return "Sawn-off Shotgun";
		case WEAPON_SHOTGSPA: // wtf? 
			return "Combat Shotgun";
		case WEAPON_UZI:
			return "UZI";
		case WEAPON_MP5:
			return "MP5";
		case WEAPON_AK47:
			return "AK47";
		case WEAPON_M4:
			return "M4";
		case WEAPON_TEC9:
			return "TEC9";
		case WEAPON_RIFLE:
			return "Rifle";
		case WEAPON_SNIPER:
			return "Sniper Rifle";
		case WEAPON_ROCKETLAUNCHER:
			return "Rocket Launcher";
		case WEAPON_HEATSEEKER:
			return "Heat Seaker";
		case WEAPON_FLAMETHROWER:
			return "Flamethrower";
		case WEAPON_MINIGUN:
			return "Minigun";
		case WEAPON_SATCHEL:
			return "Satchel Explosives";
		case WEAPON_BOMB:
			return "Bomb";
		case WEAPON_SPRAYCAN:
			return "Spray Can";
		case WEAPON_FIREEXTINGUISHER:
			return "Fire Extinguisher";
		case WEAPON_CAMERA:
			return "Camera";
		case WEAPON_NIGHTVISION:
			return "Nightvision";
		case WEAPON_INFRARED:
			return "Infrared";
		case WEAPON_PARACHUTE:
			return "Parachute";
		case 47:
			return "Fake Pistol";
		case WEAPON_VEHICLE:
			return "Vehicle";
		case 50:
			return "Helicopter Blades";
		case 51:
			return "Explosion";
		case WEAPON_DROWN:
			return "Drowned";
		case WEAPON_COLLISION:
			return "Splat";
	}
	return "";
}

int set_amxstring(AMX *amx, cell amx_addr, const char *source, int max)
{
	cell* dest = (cell *)(amx->base + (int)(((AMX_HEADER *)amx->base)->dat + amx_addr));
	cell* start = dest;
	while (max--&&*source)
		*dest++ = (cell)*source++;
	*dest = 0;
	return dest - start;
}

bool IsNumeric(const std::string& input) 
{
	return std::all_of(input.begin(), input.end(), ::isdigit);
}

bool fexists(const std::string& filename) 
{
	std::ifstream ifile(filename.c_str());
	bool ret = ifile.is_open();
	if (ret)
		ifile.close();
	return ret;
}