#pragma once
#include <string>

class CopValueConverter {
	public:
	static std::wstring ConvertToString(short value) {
		if (value < 600) return L"0 Stars";
		else if (value < 1600) return L"1 Star";
		else if (value < 3000) return L"2 Stars";
		else if (value < 5000) return L"3 Stars";
		else if (value < 8000) return L"4 Stars";
		else if (value < 12000) return L"5 Stars";
		else return L"6 Stars";
	}

	static bool AreEqual(short a, short b) {
		if (a < 600 && b < 600) return true;
		if (a >= 600 && a < 1600 && b >= 600 && b < 1600) return true;
		if (a >= 1600 && a < 3000 && b >= 1600 && b < 3000) return true;
		if (a >= 3000 && a < 5000 && b >= 3000 && b < 5000) return true;
		if (a >= 5000 && a < 8000 && b >= 5000 && b < 8000) return true;
		if (a >= 8000 && a < 12000 && b >= 8000 && b < 12000) return true;
		if (a >= 12000 && b >= 12000) return true;
		return false;
	}

protected:
	CopValueConverter() = delete;
	~CopValueConverter() = delete;
	CopValueConverter(const CopValueConverter&) = delete;
	CopValueConverter& operator=(const CopValueConverter&) = delete;
};
