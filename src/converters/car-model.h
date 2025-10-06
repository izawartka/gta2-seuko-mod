#pragma once
#include "default.h"
#include "../game/game.h"

class CarModelConverter {
public:
    static std::wstring ConvertToString(Game::CAR_MODEL4 value) {
		int32_t id = static_cast<int32_t>(value);

		switch (id) {
		case 0: return L"ALFA";
		case 1: return L"ALLARD";
		case 2: return L"AMDB4";
		case 3: return L"APC";
		case 4: return L"BANKVAN";
		case 5: return L"BMW";
		case 6: return L"BOXCAR";
		case 7: return L"BOXTRUCK";
		case 8: return L"BUG";
		case 10: return L"BUICK";
		case 11: return L"BUS";
		case 12: return L"COPCAR";
		case 13: return L"DART";
		case 14: return L"EDSEL";
		case 16: return L"FIAT";
		case 17: return L"FIRETRUK";
		case 18: return L"GRAHAM";
		case 19: return L"GT24640";
		case 21: return L"GTRUCK";
		case 22: return L"GUNJEEP";
		case 23: return L"HOTDOG";
		case 24: return L"HOTDOG_D1";
		case 25: return L"HOTDOG_D2";
		case 26: return L"HOTDOG_D3";
		case 27: return L"ICECREAM";
		case 28: return L"ISETLIMO";
		case 29: return L"ISETTA";
		case 30: return L"JEEP";
		case 31: return L"JEFFREY";
		case 32: return L"LIMO";
		case 33: return L"LIMO2";
		case 34: return L"MEDICAR";
		case 35: return L"MERC";
		case 36: return L"MESSER";
		case 37: return L"MIURA";
		case 38: return L"MONSTER";
		case 39: return L"MORGAN";
		case 40: return L"MORRIS";
		case 41: return L"PICKUP";
		case 42: return L"RTYPE";
		case 44: return L"SPIDER";
		case 45: return L"SPRITE";
		case 46: return L"STINGRAY";
		case 47: return L"STRATOS";
		case 48: return L"STRATOSB";
		case 49: return L"STRIPETB";
		case 50: return L"STYPE";
		case 51: return L"STYPECAB";
		case 52: return L"SWATVAN";
		case 53: return L"T2000GT";
		case 54: return L"TANK";
		case 55: return L"TANKER";
		case 56: return L"TAXI";
		case 57: return L"TBIRD";
		case 58: return L"TOWTRUCK";
		case 59: return L"TRAIN";
		case 60: return L"TRAINCAB";
		case 61: return L"TRAINFB";
		case 62: return L"TRANCEAM";
		case 63: return L"TRUKCAB1";
		case 64: return L"TRUKCAB2";
		case 65: return L"TRUKCONT";
		case 66: return L"TRUKTRNS";
		case 67: return L"TVVAN";
		case 68: return L"VAN";
		case 69: return L"VESPA";
		case 70: return L"VTYPE";
		case 71: return L"WBTWIN";
		case 72: return L"WRECK0";
		case 73: return L"WRECK1";
		case 74: return L"WRECK2";
		case 75: return L"WRECK3";
		case 76: return L"WRECK4";
		case 77: return L"WRECK5";
		case 78: return L"WRECK6";
		case 79: return L"WRECK7";
		case 80: return L"WRECK8";
		case 81: return L"WRECK9";
		case 82: return L"XK120";
		case 83: return L"ZCX5";
		case 84: return L"EDSELFBI";
		case 85: return L"HOTDOG_D4";
		case 86: return L"KRSNABUS";
		default: return L"???";
		}
    }

    static Game::CAR_MODEL4 ConvertFromString(std::wstring text) {
		return static_cast<Game::CAR_MODEL4>(0); // No direct editing allowed
    }

    static bool IsValidChar(std::wstring text, wchar_t c) {
		return false; // No direct editing allowed
    }

protected:
    CarModelConverter() = delete;
    ~CarModelConverter() = delete;
    CarModelConverter(const CarModelConverter&) = delete;
    CarModelConverter& operator=(const CarModelConverter&) = delete;
};