#pragma once
#include "../common.h"

template <typename T>
inline constexpr bool always_false = false;

namespace UiModule {
    // Removes trailing zeros from a floating-point string representation.
    inline void RemoveTrailingZeros(std::wstring& str) {
        if (str.empty()) return;
        auto dotPos = str.find(L'.');
        if (dotPos == std::wstring::npos) return;
        while (!str.empty() && str.back() == L'0') {
            str.pop_back();
        }
        if (!str.empty() && str.back() == L'.') {
            str.pop_back();
        }
    }

	// Converts various types to their string representation.
	template <typename T>
    inline std::wstring ToString(T value) {
        std::fesetround(FE_TONEAREST);
        if constexpr (std::is_same_v<T, Game::SCR_f>) {
			return ToString(static_cast<float>(value) / 16384.0f);
        }

		return std::to_wstring(value);
	}

	// Converts a string representation back to the specified type.
    template <typename T>
    inline T FromString(const std::wstring& str) {
        using BaseT = std::remove_cv_t<T>;
        if constexpr (std::is_same_v<BaseT, std::wstring>) {
            return str;
        }
        if constexpr (std::is_floating_point_v<BaseT>) {
            return static_cast<BaseT>(std::stof(str));
        }
        if constexpr (std::is_integral_v<BaseT>) {
            return static_cast<BaseT>(std::stoll(str));
        }
        static_assert(always_false<T>, "FromString is not implemented for this type");
        return T{};
    }

    // Explicit specialization for SCR_f
    template <>
    inline Game::SCR_f FromString<Game::SCR_f>(const std::wstring& str) {
        float f = std::stof(str);
        return static_cast<Game::SCR_f>(f * 16384.0f);
    }

	// Checks if a character is valid for the specified type.
	template <typename T>
    inline bool IsValidCharForType(char c, bool allowControlChars = false) {
        using BaseT = std::remove_cv_t<T>;
        if constexpr (std::is_same_v<BaseT, std::wstring>) {
            if (std::isprint(static_cast<unsigned char>(c))) {
                return true;
            }
            if (allowControlChars) {
                return c == '\t' || c == '\n' || c == '\r';
            }
            return false;
        }
        if constexpr (std::is_floating_point_v<BaseT>) {
            return std::isdigit(c) || (c == '-') || (c == '.');
        }
        if constexpr (std::is_integral_v<BaseT>) {
            return std::isdigit(c) || (c == '-');
        }
        static_assert(always_false<T>, "IsValidCharForType is not implemented for this type. Type name: " __FUNCSIG__);
        return false;
	}

    // Explicit specialization for SCR_f
    template <>
    inline bool IsValidCharForType<Game::SCR_f>(char c, bool /*allowControlChars*/) {
        return std::isdigit(c) || (c == '-') || (c == '.');
    }
}
