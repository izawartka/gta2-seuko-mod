#pragma once
#include "../modules/keybinding/keybinding.h"
#include "key-code.h"

template <bool gtaFontSafe = false>
class KeyConverter {
public:
	static std::wstring ConvertToString(KeyBindingModule::Key value) {
		std::wstring result = L"";
		std::wstring delimiter = gtaFontSafe ? L"  " : L" + ";
		if (value.ctrl) {
			result += L"Ctrl" + delimiter;
		}
		if (value.alt) {
			result += L"Alt" + delimiter;
		}
		if (value.shift) {
			result += L"Shift" + delimiter;
		}
		result += KeyCodeConverter<gtaFontSafe>::ConvertToString(value.keyCode);
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
