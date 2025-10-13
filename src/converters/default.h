#pragma once
#include <string>
#include <cfenv>
#include <type_traits>
#include <cassert>

template <typename T>
class DefaultConverter {
public:
	static std::wstring ConvertToString(T value) {
		std::fesetround(FE_TONEAREST);
		return std::to_wstring(value);
	}

	static T ConvertFromString(std::wstring text) {
		using BaseT = std::remove_cv_t<T>;

		if constexpr (std::is_same_v<BaseT, std::wstring>) {
			return text;
		}
		else if constexpr (std::is_floating_point_v<BaseT>) {
			return static_cast<BaseT>(std::stof(text));
		}
		else if constexpr (std::is_integral_v<BaseT>) {
			return static_cast<BaseT>(std::stoll(text));
		}
		else {
			assert(false, "Unsupported type for DefaultConverter");
			return T{};
		}
	}

	static bool IsValidChar(std::wstring text, wchar_t c) {
		using BaseT = std::remove_cv_t<T>;

		if constexpr (std::is_same_v<BaseT, std::wstring>) {
			return c >= 32 && c <= 126;
		}
		else if constexpr (std::is_floating_point_v<BaseT>) {
			bool hasDecimal = text.find(L'.') != std::wstring::npos;
			if (c == L'.') return !hasDecimal;

			bool hasLength = text.length() > 0;
			if (c == L'-' || c == L'+') return !hasLength;

			return (c >= L'0' && c <= L'9');
		}
		else if constexpr (std::is_integral_v<BaseT>) {
			bool hasLength = text.length() > 0;
			if (c == L'-' || c == L'+') return !hasLength;

			return (c >= L'0' && c <= L'9');
		}
		else {
			assert(false, "Unsupported type for DefaultConverter");
			return false;
		}
	}

	static bool AreEqual(T a, T b) {
		return a == b;
	}

protected:
	DefaultConverter() = delete;
	~DefaultConverter() = delete;
	DefaultConverter(const DefaultConverter&) = delete;
	DefaultConverter& operator=(const DefaultConverter&) = delete;
};
