#pragma once
#include <string>

class EnabledDisabledConverter {
public:
	static std::wstring ConvertToString(bool value) {
		return value ? L"Enabled" : L"Disabled";
	}

	static bool AreEqual(bool a, bool b) {
		return a == b;
	}

protected:
	EnabledDisabledConverter() = delete;
	~EnabledDisabledConverter() = delete;
	EnabledDisabledConverter(const EnabledDisabledConverter&) = delete;
	EnabledDisabledConverter& operator=(const EnabledDisabledConverter&) = delete;
};
