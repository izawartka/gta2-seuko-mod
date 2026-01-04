#pragma once
#include <string>
#include "../game/game.h"

class ObjectTypeConverter {
public:
	static std::wstring ConvertToString(Game::OBJECT_TYPE value) {
		int32_t id = static_cast<int32_t>(value);

		switch (id) {
		case 1: return L"BIN LID";
		case 2: return L"BOLLARD";
		case 3: return L"CONE";
		case 4: return L"BOXES";
		case 5: return L"BLASTER";
		case 6: return L"RUBBISH";
		case 7: return L"BIN";
		case 8: return L"ANIMATING OIL";
		case 9: return L"OIL";
		case 10: return L"MINE";
		case 11: return L"BUSH";
		case 12: return L"CRATE";
		case 13: return L"FOOTY";
		case 14: return L"HARDBOX";
		case 15: return L"NEWSDIS";
		case 16: return L"OILDRUM";
		case 17: return L"TYRE";
		case 18: return L"HYDRANT LID";
		case 19: return L"HYDRANT";
		case 20: return L"HYDRANT UNLID";
		case 21: return L"ROADBLOCK";
		case 22: return L"BENCH";
		case 23: return L"PACKAGE";
		case 24: return L"UNKNOWN OBJ";
		case 25: return L"TOWER";

		case 42: return L"EXPLODE MEDIUM";
		case 43: return L"MOVING PACKAGE";      // unofficial name
		case 44: return L"DEAD PACKAGE";        // unofficial name
		case 45: return L"MOVING BENCH";        // unofficial name
		case 46: return L"DEAD BENCH";          // unofficial name
		case 47: return L"MOVING ROADBLOCK";    // unofficial name
		case 48: return L"DEAD ROADBLOCK";      // unofficial name
		case 49: return L"MOVING HYDRANT LID";  // unofficial name
		case 50: return L"ANIMATING CRATE";     // unofficial name
		case 51: return L"DEAD CRATE";          // unofficial name
		case 52: return L"DEAD BOXES";          // unofficial name
		case 53: return L"DEAD CONE";           // unofficial name
		case 54: return L"DEAD BLASTER";        // unofficial name
		case 55: return L"DEAD BUSH";           // unofficial name
		case 56: return L"MOVING FOOTY";        // unofficial name
		case 57: return L"MOVING HARDBOX";      // unofficial name
		case 58: return L"DEAD HARDBOX";        // unofficial name
		case 59: return L"MOVING NEWSDIS";      // unofficial name
		case 60: return L"DEAD NEWSDIS";        // unofficial name
		case 61: return L"MOVING OILDRUM";      // unofficial name
		case 62: return L"DEAD OILDRUM";        // unofficial name
		case 63: return L"MOVING TYRE";         // unofficial name

		case 64: return L"MOVING COLLECT 00 (Pistol)";
		case 65: return L"MOVING COLLECT 01 (S-Uzi)";
		case 66: return L"MOVING COLLECT 02 (Rocket Launcher)";
		case 67: return L"MOVING COLLECT 03 (ElectroGun)";
		case 68: return L"MOVING COLLECT 04 (Molotov Coctail)";
		case 69: return L"MOVING COLLECT 05 (Grenade)";
		case 70: return L"MOVING COLLECT 06 (Shotgun)";
		case 71: return L"MOVING COLLECT 07 (ElectroBaton !)";
		case 72: return L"MOVING COLLECT 08 (Flamethrower)";
		case 73: return L"MOVING COLLECT 09 (Silenced S-Uzi)";
		case 74: return L"MOVING COLLECT 10 (Dual Pistol)";
		case 75: return L"MOVING COLLECT 11 (Letter L)";
		case 76: return L"MOVING COLLECT 12 (Letter M)";
		case 77: return L"MOVING COLLECT 13 (Letter N)";
		case 78: return L"MOVING COLLECT 14 (Letter O)";

		case 79: return L"MOVING COLLECT 15 (Vehicle bomb)";
		case 80: return L"MOVING COLLECT 16 (Vehicle Oil Slick)";
		case 81: return L"MOVING COLLECT 17 (Vehicle Mine)";
		case 82: return L"MOVING COLLECT 18 (Vehicle Machine Gun)";
		case 83: return L"MOVING COLLECT 19 (Tank Cannon)";
		case 84: return L"MOVING COLLECT 20 (Water Cannon)";
		case 85: return L"MOVING COLLECT 21 (Vehicle Flamethrower)";
		case 86: return L"MOVING COLLECT 22 (Jeep Turret)";
		case 87: return L"MOVING COLLECT 23 (Instant Vehicle Bomb)";
		case 88: return L"MOVING COLLECT 24 (Letter J)";
		case 89: return L"MOVING COLLECT 25 (Letter K)";
		case 90: return L"MOVING COLLECT 26 (Letter L)";
		case 91: return L"MOVING COLLECT 27 (Letter M)";

		case 92: return L"MOVING COLLECT 28 (Multiplier +1)";
		case 93: return L"MOVING COLLECT 29 (Life +1)";
		case 94: return L"MOVING COLLECT 30 (Health)";
		case 95: return L"MOVING COLLECT 31 (Armor)";
		case 96: return L"MOVING COLLECT 32 (Get Outta Jail Free Card)";
		case 97: return L"MOVING COLLECT 33 (Cop Bribe)";
		case 98: return L"MOVING COLLECT 34 (Invulnerability*)";
		case 99: return L"MOVING COLLECT 35 (Double Damage)";
		case 100: return L"MOVING COLLECT 36 (Fast Reload)";
		case 101: return L"MOVING COLLECT 37 (ElectroFingers)";
		case 102: return L"MOVING COLLECT 38 (Respect!)";
		case 103: return L"MOVING COLLECT 39 (Invisibility)";
		case 104: return L"MOVING COLLECT 40 (Instant Gang)";
		case 105: return L"MOVING COLLECT 41 (Letter N)";
		case 106: return L"MOVING COLLECT 42 (Letter O)";
		case 107: return L"MOVING COLLECT 43 (Letter P)";
		case 108: return L"MOVING COLLECT 44 (Letter Q)";

		case 109: return L"SMALL ARROW";
		case 111: return L"BLOOD SPARK";
		case 113: return L"PARTICLE SYSTEM";
		case 114: return L"FIREJET";
		case 116: return L"SMALL BROWN SKID";
		case 117: return L"SMALL GREY SKID";
		case 118: return L"SMALL RED SKID";
		case 119: return L"MEDIUM BROWN SKID";
		case 120: return L"MEDIUM GREY SKID";
		case 121: return L"MEDIUM RED SKID";
		case 122: return L"CAR CROSSING";
		case 123: return L"MOVING BUSH";
		case 124: return L"BIG WHITE SKID";
		case 125: return L"MEDIUM WHITE SKID";
		case 126: return L"SMALL WHITE SKID";
		case 128: return L"ROCKET";
		case 129: return L"BUS STOP MARKER";
		case 130: return L"CAR SHOP";
		case 131: return L"BUSY CAR SHOP";
		case 132: return L"CAR BOMB";
		case 138: return L"MOLOTOV MOVING";
		case 144: return L"HUGE RED SKID";
		case 145: return L"HUGE WHITE SKID";
		case 146: return L"HUGE BROWN SKID";
		case 147: return L"HUGE GREY SKID";
		case 148: return L"TANKTOP";
		case 149: return L"ANTENNA";
		case 151: return L"ANIMATING RUBBISH";
		case 152: return L"DEAD RUBBISH";
		case 155: return L"MOVING CONE";
		case 156: return L"MOVING BLASTER"; // unofficial name
		case 157: return L"MOVING BIN"; // unofficial name
		case 158: return L"MOVING BIN LID";
		case 161: return L"SAVE POINT"; // unofficial name
		case 163: return L"PHONE";
		case 164: return L"PHONE RINGING";
		case 165: return L"DEAD MINE"; // unofficial name // not sure about this one
		case 174: return L"PHONE DEAD";
		case 175: return L"BRIEFCASE";
		case 176: return L"RED PHONE";
		case 177: return L"RED PHONE RINGING";
		case 178: return L"YELLOW PHONE";
		case 179: return L"YELLOW PHONE RINGING";
		case 180: return L"GREEN PHONE";
		case 181: return L"GREEN PHONE RINGING";
		case 182: return L"GRENADE";
		case 192: return L"SHOT";
		case 194: return L"FLAMING BULLET";
		case 197: return L"FIRE";
		case 198: return L"WATER BULLET";

		case 200: return L"COLLECT 00 (Pistol)";
		case 201: return L"COLLECT 01 (S-Uzi)";
		case 202: return L"COLLECT 02 (Rocket Launcher)";
		case 203: return L"COLLECT 03 (ElectroGun)";
		case 204: return L"COLLECT 04 (Molotov Coctail)";
		case 205: return L"COLLECT 05 (Grenade)";
		case 206: return L"COLLECT 06 (Shotgun)";
		case 207: return L"COLLECT 07 (ElectroBaton !)";
		case 208: return L"COLLECT 08 (Flamethrower)";
		case 209: return L"COLLECT 09 (Silenced S-Uzi)";
		case 210: return L"COLLECT 10 (Dual Pistol)";
		case 211: return L"COLLECT 11 (Letter L)";
		case 212: return L"COLLECT 12 (Letter M)";
		case 213: return L"COLLECT 13 (Letter N)";
		case 214: return L"COLLECT 14 (Letter O)";
		case 215: return L"COLLECT 15 (Vehicle bomb)";
		case 216: return L"COLLECT 16 (Vehicle Oil Slick)";
		case 217: return L"COLLECT 17 (Vehicle Mine)";
		case 218: return L"COLLECT 18 (Vehicle Machine Gun)";
		case 219: return L"COLLECT 19 (Tank Cannon)";
		case 220: return L"COLLECT 20 (Water Cannon)";
		case 221: return L"COLLECT 21 (Vehicle Flamethrower)";
		case 222: return L"COLLECT 22 (Jeep Turret)";
		case 223: return L"COLLECT 23 (Instant Vehicle Bomb)";
		case 224: return L"COLLECT 24 (Letter J)";
		case 225: return L"COLLECT 25 (Letter K)";
		case 226: return L"COLLECT 26 (Letter L)";
		case 227: return L"COLLECT 27 (Letter M)";
		case 228: return L"COLLECT 28 (Multiplier +1)";
		case 229: return L"COLLECT 29 (Life +1)";
		case 230: return L"COLLECT 30 (Health)";
		case 231: return L"COLLECT 31 (Armor)";
		case 232: return L"COLLECT 32 (Get Outta Jail Free Card)";
		case 233: return L"COLLECT 33 (Cop Bribe)";
		case 234: return L"COLLECT 34 (Invulnerability*)";
		case 235: return L"COLLECT 35 (Double Damage)";
		case 236: return L"COLLECT 36 (Fast Reload)";
		case 237: return L"COLLECT 37 (ElectroFingers)";
		case 238: return L"COLLECT 38 (Respect!)";
		case 239: return L"COLLECT 39 (Invisibility)";
		case 240: return L"COLLECT 40 (Instant Gang)";
		case 241: return L"COLLECT 41 (Letter N)";
		case 242: return L"COLLECT 42 (Letter O)";
		case 243: return L"COLLECT 43 (Letter P)";
		case 244: return L"COLLECT 44 (Letter Q)";

		case 247: return L"BOMB";
		case 249: return L"BIG BROWN SKID";
		case 250: return L"BIG GREY SKID";
		case 253: return L"BIG RED SKID";
		case 254: return L"BULLET";
		case 255: return L"TRAFFIC LIGHT";
		case 256: return L"RED FOOTPRINTS";
		case 257: return L"BLOOD";
		case 258: return L"CROSSING";
		case 259: return L"SPARK";
		case 265: return L"PISTOL BULLET";
		case 266: return L"BONUS TOKEN";
		case 277: return L"TASSER BULLET";
		case 279: return L"SOUND INSTANT";
		case 280: return L"INVISIBLE TARGET";
		case 281: return L"POWERGEN";
		case 282: return L"POWERGEN DEAD";
		case 283: return L"INVISIBLE DESTRUCTIBLE";
		case 284: return L"GENLITE";
		case 285: return L"INVISIBLE DEAD";
		case 286: return L"KILL FRENZY";
		case 294: return L"TUNNEL BLOCKER";
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
