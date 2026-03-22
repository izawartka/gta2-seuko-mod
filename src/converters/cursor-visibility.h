#pragma once
#include <string>
#include "../modules/mouse/mouse.h"

class CursorVisibilityConverter {
public:
	static std::wstring ConvertToString(MouseModule::CursorVisibility value) {
		switch (value) {
		case MouseModule::CursorVisibility::Unmodified:
			return L"Unmodified";
		case MouseModule::CursorVisibility::ForceVisible:
			return L"Force Visible";
		case MouseModule::CursorVisibility::ForceInvisible:
			return L"Force Invisible";
		default:
			return L"Unknown";
		}
	}

	static bool AreEqual(MouseModule::CursorVisibility a, MouseModule::CursorVisibility b) {
		return a == b;
	}

protected:
	CursorVisibilityConverter() = delete;
	~CursorVisibilityConverter() = delete;
	CursorVisibilityConverter(const CursorVisibilityConverter&) = delete;
	CursorVisibilityConverter& operator=(const CursorVisibilityConverter&) = delete;
};
