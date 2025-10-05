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

	static short ConvertFromString(std::wstring text) {
		return 0; // No direct editing allowed
	}

	static bool IsValidChar(std::wstring text, wchar_t c) {
		return false; // No direct editing allowed
	}

protected:
	CopValueConverter() = delete;
	~CopValueConverter() = delete;
	CopValueConverter(const CopValueConverter&) = delete;
	CopValueConverter& operator=(const CopValueConverter&) = delete;
};