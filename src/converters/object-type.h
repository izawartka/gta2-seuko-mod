#pragma once
#include <string>
#include "../game/game.h"

class ObjectTypeConverter {
public:
	static std::wstring ConvertToString(Game::OBJECT_TYPE value) {
		int32_t id = static_cast<int32_t>(value);

		switch (id) {
		case 1: return L"BIN_LID";
		case 2: return L"BOLLARD";
		case 3: return L"CONE";
		case 4: return L"BOXES";
		case 5: return L"BLASTER";
		case 6: return L"RUBBISH";
		case 7: return L"BIN";
		case 8: return L"ANIMATING_OIL";
		case 9: return L"OIL";
		case 10: return L"MINE";
		case 11: return L"BUSH";
		case 12: return L"CRATE";
		case 13: return L"FOOTY";
		case 14: return L"HARDBOX";
		case 15: return L"NEWSDIS";
		case 16: return L"OILDRUM";
		case 17: return L"TYRE";
		case 18: return L"HYDRANT_LID";
		case 19: return L"HYDRANT";
		case 20: return L"HYDRANT_UNLID";
		case 21: return L"ROADBLOCK";
		case 22: return L"BENCH";
		case 23: return L"PACKAGE";
		case 24: return L"UNKNOWN_OBJ";
		case 25: return L"TOWER";

		case 42: return L"EXPLODE_MEDIUM";
		case 43: return L"MOVING_PACKAGE";      // unofficial name
		case 44: return L"DEAD_PACKAGE";        // unofficial name
		case 45: return L"MOVING_BENCH";        // unofficial name
		case 46: return L"DEAD_BENCH";          // unofficial name
		case 47: return L"MOVING_ROADBLOCK";    // unofficial name
		case 48: return L"DEAD_ROADBLOCK";      // unofficial name
		case 49: return L"MOVING_HYDRANT_LID";  // unofficial name
		case 50: return L"ANIMATING_CRATE";     // unofficial name
		case 51: return L"DEAD_CRATE";          // unofficial name
		case 52: return L"DEAD_BOXES";          // unofficial name
		case 53: return L"DEAD_CONE";           // unofficial name
		case 54: return L"DEAD_BLASTER";        // unofficial name
		case 55: return L"DEAD_BUSH";           // unofficial name
		case 56: return L"MOVING_FOOTY";        // unofficial name
		case 57: return L"MOVING_HARDBOX";      // unofficial name
		case 58: return L"DEAD_HARDBOX";        // unofficial name
		case 59: return L"MOVING_NEWSDIS";      // unofficial name
		case 60: return L"DEAD_NEWSDIS";        // unofficial name
		case 61: return L"MOVING_OILDRUM";      // unofficial name
		case 62: return L"DEAD_OILDRUM";        // unofficial name
		case 63: return L"MOVING_TYRE";         // unofficial name

		case 64: return L"MOVING_COLLECT_00 (Pistol)";
		case 65: return L"MOVING_COLLECT_01 (S-Uzi)";
		case 66: return L"MOVING_COLLECT_02 (Rocket Launcher)";
		case 67: return L"MOVING_COLLECT_03 (ElectroGun)";
		case 68: return L"MOVING_COLLECT_04 (Molotov Coctail)";
		case 69: return L"MOVING_COLLECT_05 (Grenade)";
		case 70: return L"MOVING_COLLECT_06 (Shotgun)";
		case 71: return L"MOVING_COLLECT_07 (ElectroBaton !)";
		case 72: return L"MOVING_COLLECT_08 (Flamethrower)";
		case 73: return L"MOVING_COLLECT_09 (Silenced S-Uzi)";
		case 74: return L"MOVING_COLLECT_10 (Dual Pistol)";
		case 75: return L"MOVING_COLLECT_11 (Letter L)";
		case 76: return L"MOVING_COLLECT_12 (Letter M)";
		case 77: return L"MOVING_COLLECT_13 (Letter N)";
		case 78: return L"MOVING_COLLECT_14 (Letter O)";

		case 79: return L"MOVING_COLLECT_15 (Vehicle bomb)";
		case 80: return L"MOVING_COLLECT_16 (Vehicle Oil Slick)";
		case 81: return L"MOVING_COLLECT_17 (Vehicle Mine)";
		case 82: return L"MOVING_COLLECT_18 (Vehicle Machine Gun)";
		case 83: return L"MOVING_COLLECT_19 (Tank Cannon)";
		case 84: return L"MOVING_COLLECT_20 (Water Cannon)";
		case 85: return L"MOVING_COLLECT_21 (Vehicle Flamethrower)";
		case 86: return L"MOVING_COLLECT_22 (Jeep Turret)";
		case 87: return L"MOVING_COLLECT_23 (Instant Vehicle Bomb)";
		case 88: return L"MOVING_COLLECT_24 (Letter J)";
		case 89: return L"MOVING_COLLECT_25 (Letter K)";
		case 90: return L"MOVING_COLLECT_26 (Letter L)";
		case 91: return L"MOVING_COLLECT_27 (Letter M)";

		case 92: return L"MOVING_COLLECT_28 (Multiplier +1)";
		case 93: return L"MOVING_COLLECT_29 (Life +1)";
		case 94: return L"MOVING_COLLECT_30 (Health)";
		case 95: return L"MOVING_COLLECT_31 (Armor)";
		case 96: return L"MOVING_COLLECT_32 (Get Outta Jail Free Card)";
		case 97: return L"MOVING_COLLECT_33 (Cop Bribe)";
		case 98: return L"MOVING_COLLECT_34 (Invulnerability*)";
		case 99: return L"MOVING_COLLECT_35 (Double Damage)";
		case 100: return L"MOVING_COLLECT_36 (Fast Reload)";
		case 101: return L"MOVING_COLLECT_37 (ElectroFingers)";
		case 102: return L"MOVING_COLLECT_38 (Respect!)";
		case 103: return L"MOVING_COLLECT_39 (Invisibility)";
		case 104: return L"MOVING_COLLECT_40 (Instant Gang)";
		case 105: return L"MOVING_COLLECT_41 (Letter N)";
		case 106: return L"MOVING_COLLECT_42 (Letter O)";
		case 107: return L"MOVING_COLLECT_43 (Letter P)";
		case 108: return L"MOVING_COLLECT_44 (Letter Q)";

		case 109: return L"SMALL_ARROW";
		case 111: return L"BLOOD_SPARK";
		case 113: return L"PARTICLE_SYSTEM";
		case 114: return L"FIREJET";
		case 116: return L"SMALL_BROWN_SKID";
		case 117: return L"SMALL_GREY_SKID";
		case 118: return L"SMALL_RED_SKID";
		case 119: return L"MEDIUM_BROWN_SKID";
		case 120: return L"MEDIUM_GREY_SKID";
		case 121: return L"MEDIUM_RED_SKID";
		case 122: return L"CAR_CROSSING";
		case 123: return L"MOVING_BUSH";
		case 124: return L"BIG_WHITE_SKID";
		case 125: return L"MEDIUM_WHITE_SKID";
		case 126: return L"SMALL_WHITE_SKID";
		case 128: return L"ROCKET";
		case 129: return L"BUS_STOP_MARKER";
		case 130: return L"CAR_SHOP";
		case 131: return L"BUSY_CAR_SHOP";
		case 132: return L"CAR_BOMB";
		case 138: return L"MOLOTOV_MOVING";
		case 144: return L"HUGE_RED_SKID";
		case 145: return L"HUGE_WHITE_SKID";
		case 146: return L"HUGE_BROWN_SKID";
		case 147: return L"HUGE_GREY_SKID";
		case 148: return L"TANKTOP";
		case 149: return L"ANTENNA";
		case 151: return L"ANIMATING_RUBBISH";
		case 152: return L"DEAD_RUBBISH";
		case 155: return L"MOVING_CONE";
		case 156: return L"MOVING_BLASTER"; // unofficial name
		case 157: return L"MOVING_BIN"; // unofficial name
		case 158: return L"MOVING_BIN_LID";
		case 161: return L"SAVE_POINT"; // unofficial name
		case 163: return L"PHONE";
		case 164: return L"PHONE_RINGING";
		case 165: return L"DEAD_MINE"; // unofficial name // not sure about this one
		case 174: return L"PHONE_DEAD";
		case 175: return L"BRIEFCASE";
		case 176: return L"RED_PHONE";
		case 177: return L"RED_PHONE_RINGING";
		case 178: return L"YELLOW_PHONE";
		case 179: return L"YELLOW_PHONE_RINGING";
		case 180: return L"GREEN_PHONE";
		case 181: return L"GREEN_PHONE_RINGING";
		case 182: return L"GRENADE";
		case 192: return L"SHOT";
		case 194: return L"FLAMING_BULLET";
		case 197: return L"FIRE";
		case 198: return L"WATER_BULLET";

		case 200: return L"COLLECT_00 (Pistol)";
		case 201: return L"COLLECT_01 (S-Uzi)";
		case 202: return L"COLLECT_02 (Rocket Launcher)";
		case 203: return L"COLLECT_03 (ElectroGun)";
		case 204: return L"COLLECT_04 (Molotov Coctail)";
		case 205: return L"COLLECT_05 (Grenade)";
		case 206: return L"COLLECT_06 (Shotgun)";
		case 207: return L"COLLECT_07 (ElectroBaton !)";
		case 208: return L"COLLECT_08 (Flamethrower)";
		case 209: return L"COLLECT_09 (Silenced S-Uzi)";
		case 210: return L"COLLECT_10 (Dual Pistol)";
		case 211: return L"COLLECT_11 (Letter L)";
		case 212: return L"COLLECT_12 (Letter M)";
		case 213: return L"COLLECT_13 (Letter N)";
		case 214: return L"COLLECT_14 (Letter O)";
		case 215: return L"COLLECT_15 (Vehicle bomb)";
		case 216: return L"COLLECT_16 (Vehicle Oil Slick)";
		case 217: return L"COLLECT_17 (Vehicle Mine)";
		case 218: return L"COLLECT_18 (Vehicle Machine Gun)";
		case 219: return L"COLLECT_19 (Tank Cannon)";
		case 220: return L"COLLECT_20 (Water Cannon)";
		case 221: return L"COLLECT_21 (Vehicle Flamethrower)";
		case 222: return L"COLLECT_22 (Jeep Turret)";
		case 223: return L"COLLECT_23 (Instant Vehicle Bomb)";
		case 224: return L"COLLECT_24 (Letter J)";
		case 225: return L"COLLECT_25 (Letter K)";
		case 226: return L"COLLECT_26 (Letter L)";
		case 227: return L"COLLECT_27 (Letter M)";
		case 228: return L"COLLECT_28 (Multiplier +1)";
		case 229: return L"COLLECT_29 (Life +1)";
		case 230: return L"COLLECT_30 (Health)";
		case 231: return L"COLLECT_31 (Armor)";
		case 232: return L"COLLECT_32 (Get Outta Jail Free Card)";
		case 233: return L"COLLECT_33 (Cop Bribe)";
		case 234: return L"COLLECT_34 (Invulnerability*)";
		case 235: return L"COLLECT_35 (Double Damage)";
		case 236: return L"COLLECT_36 (Fast Reload)";
		case 237: return L"COLLECT_37 (ElectroFingers)";
		case 238: return L"COLLECT_38 (Respect!)";
		case 239: return L"COLLECT_39 (Invisibility)";
		case 240: return L"COLLECT_40 (Instant Gang)";
		case 241: return L"COLLECT_41 (Letter N)";
		case 242: return L"COLLECT_42 (Letter O)";
		case 243: return L"COLLECT_43 (Letter P)";
		case 244: return L"COLLECT_44 (Letter Q)";

		case 247: return L"BOMB";
		case 249: return L"BIG_BROWN_SKID";
		case 250: return L"BIG_GREY_SKID";
		case 253: return L"BIG_RED_SKID";
		case 254: return L"BULLET";
		case 255: return L"TRAFFIC_LIGHT";
		case 256: return L"RED_FOOTPRINTS";
		case 257: return L"BLOOD";
		case 258: return L"CROSSING";
		case 259: return L"SPARK";
		case 265: return L"PISTOL_BULLET";
		case 266: return L"BONUS_TOKEN";
		case 277: return L"TASSER_BULLET";
		case 279: return L"SOUND_INSTANT";
		case 280: return L"INVISIBLE_TARGET";
		case 281: return L"POWERGEN";
		case 282: return L"POWERGEN_DEAD";
		case 283: return L"INVISIBLE_DESTRUCTIBLE";
		case 284: return L"GENLITE";
		case 285: return L"INVISIBLE_DEAD";
		case 286: return L"KILL_FRENZY";
		case 294: return L"TUNNEL_BLOCKER";
		case 295: return L"REMOTE";

		default:
			return L"Unknown " + std::to_wstring(id);
		}

	}

	static bool AreEqual(Game::OBJECT_TYPE a, Game::OBJECT_TYPE b) {
		return a == b;
	}

protected:
	ObjectTypeConverter() = delete;
	~ObjectTypeConverter() = delete;
	ObjectTypeConverter(const ObjectTypeConverter&) = delete;
	ObjectTypeConverter& operator=(const ObjectTypeConverter&) = delete;
};
