#pragma once
#include <string>
#include <cfenv>
#include <type_traits>
#include <cassert>
#include <cmath>

template <typename T, size_t precision = 3>
class FloatConverter {
public:
	static std::wstring ConvertToString(T value) {
		using BaseT = std::remove_cv_t<T>;
		std::fesetround(FE_TONEAREST);

		if constexpr (std::is_floating_point_v<BaseT>) {
			BaseT roundFactor = static_cast<BaseT>(std::pow(10, precision));
			BaseT roundedValue = std::round(value * roundFactor) / roundFactor;

			std::wstring text = std::to_wstring(roundedValue);
			size_t decimalPos = text.find(L'.');
			if (decimalPos == std::wstring::npos) return text;
			size_t endPos = decimalPos + 1 + precision;
			if (endPos < text.length()) {
				text = text.substr(0, endPos);
			}
			return text;
		}
		else {
#ifdef DEBUG
			return L"<Unsupported value>";
#else
			return L"";
#endif // DEBUG
		}
	}

	static T ConvertFromString(std::wstring text) {
		using BaseT = std::remove_cv_t<T>;
		std::fesetround(FE_TONEAREST);

		if constexpr (std::is_floating_point_v<BaseT>) {
			if (text.empty()) return T{};
			return static_cast<BaseT>(std::stod(text));
		}
		else {
			assert(false && "Unsupported type for FloatConverter");
			return T{};
		}
	}

	static bool IsValidChar(std::wstring text, wchar_t c) {
		using BaseT = std::remove_cv_t<T>;
		
		if constexpr (std::is_floating_point_v<BaseT>) {
			bool hasDecimal = text.find(L'.') != std::wstring::npos;
			if (c == L'.') return !hasDecimal;

			bool hasLength = text.length() > 0;
			if (c == L'-') return !hasLength;

			return (c >= L'0' && c <= L'9');
		}
		else {
			assert(false && "Unsupported type for FloatConverter");
			return false;
		}
	}

	static bool AreEqual(T a, T b) {
		T diff = std::fabs(a - b);
		T epsilon = static_cast<T>(std::pow(10, -static_cast<int>(precision)));
		return diff < epsilon;
	}

protected:
	FloatConverter() = delete;
	~FloatConverter() = delete;
	FloatConverter(const FloatConverter&) = delete;
	FloatConverter& operator=(const FloatConverter&) = delete;
};
