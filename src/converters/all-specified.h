#pragma once
#include <string>

class AllSpecifiedConverter {
public:
	static std::wstring ConvertToString(bool value) {
		return value ? L"All" : L"Specified";
	}

	static bool AreEqual(bool a, bool b) {
		return a == b;
	}

protected:
	AllSpecifiedConverter() = delete;
	~AllSpecifiedConverter() = delete;
	AllSpecifiedConverter(const AllSpecifiedConverter&) = delete;
	AllSpecifiedConverter& operator=(const AllSpecifiedConverter&) = delete;
};
