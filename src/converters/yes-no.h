#pragma once
#include <string>

class YesNoConverter {
public:
	static std::wstring ConvertToString(bool value) {
		return value ? L"Yes" : L"No";
	}

	static bool AreEqual(bool a, bool b) {
		return a == b;
	}

protected:
	YesNoConverter() = delete;
	~YesNoConverter() = delete;
	YesNoConverter(const YesNoConverter&) = delete;
	YesNoConverter& operator=(const YesNoConverter&) = delete;
};
