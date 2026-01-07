#pragma once
#include <string>
#include <cfenv>
#include <type_traits>
#include <cmath>

template <typename T, size_t precision = 3,
	bool Supported = std::is_floating_point_v<std::remove_cv_t<T>>
>
class FloatConverter;

// Specialization for supported types
template <typename T, size_t precision>
class FloatConverter<T, precision, true> {
public:
	static std::wstring ConvertToString(T value) {
		using BaseT = std::remove_cv_t<T>;
		std::fesetround(FE_TONEAREST);

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

	static T ConvertFromString(std::wstring text) {
		using BaseT = std::remove_cv_t<T>;
		std::fesetround(FE_TONEAREST);

		if (text.empty()) return T{};
		return static_cast<BaseT>(std::stod(text));
	}

	static bool IsValidChar(std::wstring text, wchar_t c) {
		bool hasDecimal = text.find(L'.') != std::wstring::npos;
		if (c == L'.') return !hasDecimal;

		bool hasLength = text.length() > 0;
		if (c == L'-') return !hasLength;

		return (c >= L'0' && c <= L'9');
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

// Specialization for unsupported types
template <typename T, size_t precision>
class FloatConverter<T, precision, false> {
public:
	static std::wstring ConvertToString(T value) {
		return L"<Unsupported value>";
	}

protected:
	FloatConverter() = delete;
	~FloatConverter() = delete;
	FloatConverter(const FloatConverter&) = delete;
	FloatConverter& operator=(const FloatConverter&) = delete;
};
