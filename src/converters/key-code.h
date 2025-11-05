#pragma once
#include "../game/game.h"
#include <string>

template <bool descriptiveNames = false>
class KeyCodeConverter {
public:
	static std::wstring ConvertToString(Game::KeyCode value) {
		switch (value)
		{
		case Game::KeyCode::DIK_NONE: return L"None";
		case Game::KeyCode::DIK_ESCAPE: return L"Escape";
		case Game::KeyCode::DIK_1: return L"1";
		case Game::KeyCode::DIK_2: return L"2";
		case Game::KeyCode::DIK_3: return L"3";
		case Game::KeyCode::DIK_4: return L"4";
		case Game::KeyCode::DIK_5: return L"5";
		case Game::KeyCode::DIK_6: return L"6";
		case Game::KeyCode::DIK_7: return L"7";
		case Game::KeyCode::DIK_8: return L"8";
		case Game::KeyCode::DIK_9: return L"9";
		case Game::KeyCode::DIK_0: return L"0";
		case Game::KeyCode::DIK_MINUS: return descriptiveNames ? L"Minus" : L"-";
		case Game::KeyCode::DIK_EQUALS: return descriptiveNames ? L"Equals" : L"=";
		case Game::KeyCode::DIK_BACK: return L"Backspace";
		case Game::KeyCode::DIK_TAB: return L"Tab";
		case Game::KeyCode::DIK_Q: return L"Q";
		case Game::KeyCode::DIK_W: return L"W";
		case Game::KeyCode::DIK_E: return L"E";
		case Game::KeyCode::DIK_R: return L"R";
		case Game::KeyCode::DIK_T: return L"T";
		case Game::KeyCode::DIK_Y: return L"Y";
		case Game::KeyCode::DIK_U: return L"U";
		case Game::KeyCode::DIK_I: return L"I";
		case Game::KeyCode::DIK_O: return L"O";
		case Game::KeyCode::DIK_P: return L"P";
		case Game::KeyCode::DIK_LBRACKET: return descriptiveNames ? L"Left Bracket" : L"[";
		case Game::KeyCode::DIK_RBRACKET: return descriptiveNames ? L"Right Bracket" : L"]";
		case Game::KeyCode::DIK_RETURN: return L"Enter";
		case Game::KeyCode::DIK_LCONTROL: return L"Left Ctrl";
		case Game::KeyCode::DIK_A: return L"A";
		case Game::KeyCode::DIK_S: return L"S";
		case Game::KeyCode::DIK_D: return L"D";
		case Game::KeyCode::DIK_F: return L"F";
		case Game::KeyCode::DIK_G: return L"G";
		case Game::KeyCode::DIK_H: return L"H";
		case Game::KeyCode::DIK_J: return L"J";
		case Game::KeyCode::DIK_K: return L"K";
		case Game::KeyCode::DIK_L: return L"L";
		case Game::KeyCode::DIK_SEMICOLON: return descriptiveNames ? L"Semicolon" : L";";
		case Game::KeyCode::DIK_APOSTROPHE: return descriptiveNames ? L"Apostrophe" : L"'";
		case Game::KeyCode::DIK_GRAVE: return descriptiveNames ? L"Grave" : L"`";
		case Game::KeyCode::DIK_LSHIFT: return L"Left Shift";
		case Game::KeyCode::DIK_BACKSLASH: return descriptiveNames ? L"Backslash" : L"\\";
		case Game::KeyCode::DIK_Z: return L"Z";
		case Game::KeyCode::DIK_X: return L"X";
		case Game::KeyCode::DIK_C: return L"C";
		case Game::KeyCode::DIK_V: return L"V";
		case Game::KeyCode::DIK_B: return L"B";
		case Game::KeyCode::DIK_N: return L"N";
		case Game::KeyCode::DIK_M: return L"M";
		case Game::KeyCode::DIK_COMMA: return descriptiveNames ? L"Comma" : L",";
		case Game::KeyCode::DIK_PERIOD: return descriptiveNames ? L"Period" : L".";
		case Game::KeyCode::DIK_SLASH: return descriptiveNames ? L"Slash" : L"/";
		case Game::KeyCode::DIK_RSHIFT: return L"Right Shift";
		case Game::KeyCode::DIK_MULTIPLY: return descriptiveNames ? L"Num Multiply" : L"Num *";
		case Game::KeyCode::DIK_LMENU: return L"Left Alt";
		case Game::KeyCode::DIK_SPACE: return L"Space";
		case Game::KeyCode::DIK_CAPITAL: return L"Caps Lock";
		case Game::KeyCode::DIK_F1: return L"F1";
		case Game::KeyCode::DIK_F2: return L"F2";
		case Game::KeyCode::DIK_F3: return L"F3";
		case Game::KeyCode::DIK_F4: return L"F4";
		case Game::KeyCode::DIK_F5: return L"F5";
		case Game::KeyCode::DIK_F6: return L"F6";
		case Game::KeyCode::DIK_F7: return L"F7";
		case Game::KeyCode::DIK_F8: return L"F8";
		case Game::KeyCode::DIK_F9: return L"F9";
		case Game::KeyCode::DIK_F10: return L"F10";
		case Game::KeyCode::DIK_NUMLOCK: return L"Num Lock";
		case Game::KeyCode::DIK_SCROLL: return L"Scroll Lock";
		case Game::KeyCode::DIK_NUMPAD7: return L"Num 7";
		case Game::KeyCode::DIK_NUMPAD8: return L"Num 8";
		case Game::KeyCode::DIK_NUMPAD9: return L"Num 9";
		case Game::KeyCode::DIK_SUBTRACT: return descriptiveNames ? L"Num Subtract" : L"Num -";
		case Game::KeyCode::DIK_NUMPAD4: return L"Num 4";
		case Game::KeyCode::DIK_NUMPAD5: return L"Num 5";
		case Game::KeyCode::DIK_NUMPAD6: return L"Num 6";
		case Game::KeyCode::DIK_ADD: return descriptiveNames ? L"Num Add" : L"Num +";
		case Game::KeyCode::DIK_NUMPAD1: return L"Num 1";
		case Game::KeyCode::DIK_NUMPAD2: return L"Num 2";
		case Game::KeyCode::DIK_NUMPAD3: return L"Num 3";
		case Game::KeyCode::DIK_NUMPAD0: return L"Num 0";
		case Game::KeyCode::DIK_DECIMAL: return descriptiveNames ? L"Num Decimal" : L"Num .";
		case Game::KeyCode::DIK_OEM_102: return L"OEM 102";
		case Game::KeyCode::DIK_F11: return L"F11";
		case Game::KeyCode::DIK_F12: return L"F12";
		case Game::KeyCode::DIK_F13: return L"F13";
		case Game::KeyCode::DIK_F14: return L"F14";
		case Game::KeyCode::DIK_F15: return L"F15";
		case Game::KeyCode::DIK_KANA: return L"Kana";
		case Game::KeyCode::DIK_ABNT_C1: return L"ABNT C1";
		case Game::KeyCode::DIK_CONVERT: return L"Convert";
		case Game::KeyCode::DIK_NOCONVERT: return L"NoConvert";
		case Game::KeyCode::DIK_YEN: return L"Yen";
		case Game::KeyCode::DIK_ABNT_C2: return L"ABNT C2";
		case Game::KeyCode::DIK_NUMPADEQUALS: return descriptiveNames ? L"Num Equals" : L"Num =";
		case Game::KeyCode::DIK_PREVTRACK: return L"Prev Track";
		case Game::KeyCode::DIK_AT: return descriptiveNames ? L"At" : L"@";
		case Game::KeyCode::DIK_COLON: return descriptiveNames ? L"Colon" : L":";
		case Game::KeyCode::DIK_UNDERLINE: return descriptiveNames ? L"Underline" : L"_";
		case Game::KeyCode::DIK_KANJI: return L"Kanji";
		case Game::KeyCode::DIK_STOP: return L"Stop";
		case Game::KeyCode::DIK_AX: return L"AX";
		case Game::KeyCode::DIK_UNLABELED: return L"Unlabeled";
		case Game::KeyCode::DIK_NEXTTRACK: return L"Next Track";
		case Game::KeyCode::DIK_NUMPADENTER: return L"Num Enter";
		case Game::KeyCode::DIK_RCONTROL: return L"Right Ctrl";
		case Game::KeyCode::DIK_MUTE: return L"Mute";
		case Game::KeyCode::DIK_CALCULATOR: return L"Calculator";
		case Game::KeyCode::DIK_PLAYPAUSE: return L"Play/Pause";
		case Game::KeyCode::DIK_MEDIASTOP: return L"Media Stop";
		case Game::KeyCode::DIK_VOLUMEDOWN: return descriptiveNames ? L"Volume Down" : L"Volume -";
		case Game::KeyCode::DIK_VOLUMEUP: return descriptiveNames ? L"Volume Up" : L"Volume +";
		case Game::KeyCode::DIK_WEBHOME: return L"Web Home";
		case Game::KeyCode::DIK_NUMPADCOMMA: return descriptiveNames ? L"Num Comma" : L"Num ,";
		case Game::KeyCode::DIK_DIVIDE: return descriptiveNames ? L"Num Divide" : L"Num /";
		case Game::KeyCode::DIK_SYSRQ: return L"SysReq";
		case Game::KeyCode::DIK_RMENU: return L"Right Alt";
		case Game::KeyCode::DIK_PAUSE: return L"Pause";
		case Game::KeyCode::DIK_HOME: return L"Home";
		case Game::KeyCode::DIK_UP: return L"Up";
		case Game::KeyCode::DIK_PRIOR: return L"Page Up";
		case Game::KeyCode::DIK_LEFT: return L"Left";
		case Game::KeyCode::DIK_RIGHT: return L"Right";
		case Game::KeyCode::DIK_END: return L"End";
		case Game::KeyCode::DIK_DOWN: return L"Down";
		case Game::KeyCode::DIK_NEXT: return L"Page Down";
		case Game::KeyCode::DIK_INSERT: return L"Insert";
		case Game::KeyCode::DIK_DELETE: return L"Delete";
		case Game::KeyCode::DIK_LWIN: return L"Left Win";
		case Game::KeyCode::DIK_RWIN: return L"Right Win";
		case Game::KeyCode::DIK_APPS: return L"App Menu";
		case Game::KeyCode::DIK_POWER: return L"Power";
		case Game::KeyCode::DIK_SLEEP: return L"Sleep";
		case Game::KeyCode::DIK_WAKE: return L"Wake";
		case Game::KeyCode::DIK_WEBSEARCH: return L"Web Search";
		case Game::KeyCode::DIK_WEBFAVORITES: return L"Web Favorites";
		case Game::KeyCode::DIK_WEBREFRESH: return L"Web Refresh";
		case Game::KeyCode::DIK_WEBSTOP: return L"Web Stop";
		case Game::KeyCode::DIK_WEBFORWARD: return L"Web Forward";
		case Game::KeyCode::DIK_WEBBACK: return L"Web Back";
		case Game::KeyCode::DIK_MYCOMPUTER: return L"My Computer";
		case Game::KeyCode::DIK_MAIL: return L"Mail";
		case Game::KeyCode::DIK_MEDIASELECT: return L"Media Select";
		}

		// fallback: unknown code -> show numeric hex value
		std::wstring result = L"Unknown (";
		result += std::to_wstring(static_cast<int>(value));
		result += L")";
		return result;
	}

	static bool AreEqual(Game::KeyCode a, Game::KeyCode b) {
		return a == b;
	}

protected:
	KeyCodeConverter() = delete;
	~KeyCodeConverter() = delete;
	KeyCodeConverter(const KeyCodeConverter&) = delete;
	KeyCodeConverter& operator=(const KeyCodeConverter&) = delete;
};
