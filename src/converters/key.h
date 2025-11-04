#pragma once
#include "../modules/keybinding/keybinding.h"
#include "key-code.h"

class KeyConverter {
public:
	static std::wstring ConvertToString(KeyBindingModule::Key value) {
		std::wstring result = L"";
		if (value.ctrl) {
			result += L"Ctrl + ";
		}
		if (value.alt) {
			result += L"Alt + ";
		}
		if (value.shift) {
			result += L"Shift + ";
		}
		result += KeyCodeConverter::ConvertToString(value.keyCode);
		return result;
	}

	static bool AreEqual(KeyBindingModule::Key a, KeyBindingModule::Key b) {
		return a == b;
	}

protected:
	KeyConverter() = delete;
	~KeyConverter() = delete;
	KeyConverter(const KeyConverter&) = delete;
	KeyConverter& operator=(const KeyConverter&) = delete;
};
