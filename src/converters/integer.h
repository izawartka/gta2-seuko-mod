#pragma once
#include <string>
#include <cfenv>
#include <type_traits>
#include <limits>

template <typename T,
	bool Supported = std::is_integral_v<std::remove_cv_t<T>>
>
class IntegerConverter;

// Specialization for supported types
template <typename T>
class IntegerConverter<T, true> {
public:
	static std::wstring ConvertToString(T value) {
		using BaseT = std::remove_cv_t<T>;
		std::fesetround(FE_TONEAREST);

		return std::to_wstring(value);
	}

	static T ConvertFromString(std::wstring text) {
		using BaseT = std::remove_cv_t<T>;
		std::fesetround(FE_TONEAREST);

		if (text.empty()) return static_cast<BaseT>(0);

		using longT = std::conditional_t<std::is_signed_v<BaseT>, long long, unsigned long long>;
		longT value = 0;
		if constexpr (std::is_signed_v<BaseT>) {
			value = std::stoll(text);
		}
		else {
			value = std::stoull(text);
		}

		value = std::min(value, static_cast<longT>(std::numeric_limits<BaseT>::max()));
		value = std::max(value, static_cast<longT>(std::numeric_limits<BaseT>::min()));
		return static_cast<BaseT>(value);
	}

	static bool IsValidChar(std::wstring text, wchar_t c) {
		using BaseT = std::remove_cv_t<T>;

		if constexpr (std::is_signed_v<BaseT>) {
			bool hasLength = text.length() > 0;
			if (c == L'-') return !hasLength;
		}

		return (c >= L'0' && c <= L'9');
	}

protected:
	IntegerConverter() = delete;
	~IntegerConverter() = delete;
	IntegerConverter(const IntegerConverter&) = delete;
	IntegerConverter& operator=(const IntegerConverter&) = delete;
};

// Specialization for unsupported types
template <typename T>
class IntegerConverter<T, false> {
public:
	static std::wstring ConvertToString(T value) {
		return L"<Unsupported value>";
	}

protected:
	IntegerConverter() = delete;
	~IntegerConverter() = delete;
	IntegerConverter(const IntegerConverter&) = delete;
	IntegerConverter& operator=(const IntegerConverter&) = delete;
};
