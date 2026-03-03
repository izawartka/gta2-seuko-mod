#pragma once
#include <vector>
#include "camera-easy-mode.h"

class CameraEasyModesListConverter {
	using CameraEasyMode = ModMenuModule::Utils::CameraEasyMode::CameraEasyMode;
public:
	static std::wstring ConvertToString(std::vector<CameraEasyMode> value) {
		if (value.empty()) return L"Empty";

		std::wstring result;
		for(auto it = value.begin(); it != value.end(); ++it) {
			std::wstring modeStr = CameraEasyModeConverter::ConvertToString(*it);
			if (it != value.begin()) result += L", ";
			result += modeStr;
		}

		return result;
	}

	static bool AreEqual(std::vector<CameraEasyMode> a, std::vector<CameraEasyMode> b) {
		if (a.size() != b.size()) return false;
		for (size_t i = 0; i < a.size(); ++i) {
			if (a[i] != b[i]) return false;
		}
		return true;
	}

protected:
	CameraEasyModesListConverter() = delete;
	~CameraEasyModesListConverter() = delete;
	CameraEasyModesListConverter(const CameraEasyModesListConverter&) = delete;
	CameraEasyModesListConverter& operator=(const CameraEasyModesListConverter&) = delete;
};
