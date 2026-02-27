#pragma once
#include <string>
#include "../modules/modmenu/cheats/mouse-control.h"

class MouseControlCheatModeConverter {
public:
	static std::wstring ConvertToString(ModMenuModule::MouseControlCheatMode value) {

		switch (value) {
		case ModMenuModule::MouseControlCheatMode::Rotate:
			return L"Rotate";
		case ModMenuModule::MouseControlCheatMode::PointAt:
			return L"Point at";
		default: return L"???";
		}
	}

	static bool AreEqual(ModMenuModule::MouseControlCheatMode a, ModMenuModule::MouseControlCheatMode b) {
		return a == b;
	}

protected:
	MouseControlCheatModeConverter() = delete;
	~MouseControlCheatModeConverter() = delete;
	MouseControlCheatModeConverter(const MouseControlCheatModeConverter&) = delete;
	MouseControlCheatModeConverter& operator=(const MouseControlCheatModeConverter&) = delete;
};
