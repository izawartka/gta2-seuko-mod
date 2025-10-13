#pragma once
#include <string>

class CarRemapConverter {
public:
	static std::wstring ConvertToString(short value) {

		switch (value) {
		case -1: return L"Default";
		case 0: return L"Alpine blue";
		case 1: return L"Black";
		case 2: return L"Black 2";
		case 3: return L"Blue";
		case 4: return L"Blue grey";
		case 5: return L"B. green";
		case 6: return L"Brite red";
		case 7: return L"Brown";
		case 8: return L"Browner";
		case 9: return L"B. silver";
		case 10: return L"B. yellow";
		case 11: return L"Cream";
		case 12: return L"Cyan";
		case 13: return L"D. beige";
		case 14: return L"D. blue";
		case 15: return L"Deep blue";
		case 16: return L"D. green";
		case 17: return L"D. red";
		case 18: return L"Drust";
		case 19: return L"Gold";
		case 20: return L"Green";
		case 21: return L"Grey";
		case 22: return L"L. brown";
		case 23: return L"Olive";
		case 24: return L"Orange";
		case 25: return L"Pale blue";
		case 26: return L"Pink red";
		case 27: return L"Purple";
		case 28: return L"Red";
		case 29: return L"Rust";
		case 30: return L"Silver";
		case 31: return L"Sky blue";
		case 32: return L"Turquoise";
		case 33: return L"White";
		case 34: return L"Whiter";
		case 35: return L"Yellow";
		case 36: return L"Unknown 36"; // zzzbank
		default: return L"???";
		}
	}

	static bool AreEqual(short a, short b) {
		return a == b;
	}

protected:
	CarRemapConverter() = delete;
	~CarRemapConverter() = delete;
	CarRemapConverter(const CarRemapConverter&) = delete;
	CarRemapConverter& operator=(const CarRemapConverter&) = delete;
};
