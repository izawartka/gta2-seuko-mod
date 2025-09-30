#include "keycode.h"

char Game::GetCharFromKeyCode(KeyCode keyCode, bool shift, bool capsLock)
{
    auto letter = [&](char upper)->char {
        return (capsLock ^ shift) ? upper : static_cast<char>(upper + ('a' - 'A'));
    };

    switch (keyCode) {
    case KeyCode::DIK_1: return shift ? '!' : '1';
    case KeyCode::DIK_2: return shift ? '@' : '2';
    case KeyCode::DIK_3: return shift ? '#' : '3';
    case KeyCode::DIK_4: return shift ? '$' : '4';
    case KeyCode::DIK_5: return shift ? '%' : '5';
    case KeyCode::DIK_6: return shift ? '^' : '6';
    case KeyCode::DIK_7: return shift ? '&' : '7';
    case KeyCode::DIK_8: return shift ? '*' : '8';
    case KeyCode::DIK_9: return shift ? '(' : '9';
    case KeyCode::DIK_0: return shift ? ')' : '0';

    case KeyCode::DIK_MINUS:     return shift ? '_' : '-';
    case KeyCode::DIK_EQUALS:    return shift ? '+' : '=';
    case KeyCode::DIK_LBRACKET:  return shift ? '{' : '[';
    case KeyCode::DIK_RBRACKET:  return shift ? '}' : ']';
    case KeyCode::DIK_BACKSLASH: return shift ? '|' : '\\';
    case KeyCode::DIK_SEMICOLON: return shift ? ':' : ';';
    case KeyCode::DIK_APOSTROPHE:return shift ? '"' : '\'';
    case KeyCode::DIK_GRAVE:     return shift ? '~' : '`';
    case KeyCode::DIK_COMMA:     return shift ? '<' : ',';
    case KeyCode::DIK_PERIOD:    return shift ? '>' : '.';
    case KeyCode::DIK_SLASH:     return shift ? '?' : '/';

    case KeyCode::DIK_SPACE: return ' ';
    case KeyCode::DIK_TAB:   return '\t';
    case KeyCode::DIK_RETURN:return '\n';

    case KeyCode::DIK_NUMPAD0: return '0';
    case KeyCode::DIK_NUMPAD1: return '1';
    case KeyCode::DIK_NUMPAD2: return '2';
    case KeyCode::DIK_NUMPAD3: return '3';
    case KeyCode::DIK_NUMPAD4: return '4';
    case KeyCode::DIK_NUMPAD5: return '5';
    case KeyCode::DIK_NUMPAD6: return '6';
    case KeyCode::DIK_NUMPAD7: return '7';
    case KeyCode::DIK_NUMPAD8: return '8';
    case KeyCode::DIK_NUMPAD9: return '9';
    case KeyCode::DIK_DECIMAL:  return '.';
    case KeyCode::DIK_ADD:      return '+';
    case KeyCode::DIK_SUBTRACT: return '-';
    case KeyCode::DIK_MULTIPLY: return '*';
    case KeyCode::DIK_DIVIDE:   return '/';
    case KeyCode::DIK_NUMPADCOMMA: return ',';
    case KeyCode::DIK_NUMPADENTER: return '\n';
    case KeyCode::DIK_NUMPADEQUALS: return '=';

    case KeyCode::DIK_A: return letter('A');
    case KeyCode::DIK_B: return letter('B');
    case KeyCode::DIK_C: return letter('C');
    case KeyCode::DIK_D: return letter('D');
    case KeyCode::DIK_E: return letter('E');
    case KeyCode::DIK_F: return letter('F');
    case KeyCode::DIK_G: return letter('G');
    case KeyCode::DIK_H: return letter('H');
    case KeyCode::DIK_I: return letter('I');
    case KeyCode::DIK_J: return letter('J');
    case KeyCode::DIK_K: return letter('K');
    case KeyCode::DIK_L: return letter('L');
    case KeyCode::DIK_M: return letter('M');
    case KeyCode::DIK_N: return letter('N');
    case KeyCode::DIK_O: return letter('O');
    case KeyCode::DIK_P: return letter('P');
    case KeyCode::DIK_Q: return letter('Q');
    case KeyCode::DIK_R: return letter('R');
    case KeyCode::DIK_S: return letter('S');
    case KeyCode::DIK_T: return letter('T');
    case KeyCode::DIK_U: return letter('U');
    case KeyCode::DIK_V: return letter('V');
    case KeyCode::DIK_W: return letter('W');
    case KeyCode::DIK_X: return letter('X');
    case KeyCode::DIK_Y: return letter('Y');
    case KeyCode::DIK_Z: return letter('Z');

    case KeyCode::DIK_OEM_102: return shift ? '>' : '<'; // common on non-US 102-key
    case KeyCode::DIK_ABNT_C1: return shift ? '?' : '/'; // brazilian
    case KeyCode::DIK_ABNT_C2: return ',';               // brazilian keypad dot
    case KeyCode::DIK_YEN:      return '\\';
    case KeyCode::DIK_AT:       return '@';
    case KeyCode::DIK_COLON:    return ':';
    case KeyCode::DIK_UNDERLINE:return '_';

    default:
        return '\0';
    }
}
