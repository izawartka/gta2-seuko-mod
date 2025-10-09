#pragma once
#include <string>

class YesNoConverter {
public:
	static std::wstring ConvertToString(bool value) {
		return value ? L"Yes" : L"No";
	}

	static bool ConvertFromString(std::wstring text) {
		return false; // No direct editing allowed
	}

	static bool IsValidChar(std::wstring text, wchar_t c) {
		return false; // No direct editing allowed
	}

protected:
	YesNoConverter() = delete;
	~YesNoConverter() = delete;
	YesNoConverter(const YesNoConverter&) = delete;
	YesNoConverter& operator=(const YesNoConverter&) = delete;
};
