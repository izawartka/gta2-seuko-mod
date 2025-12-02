#pragma once
#include "float.h"
#include <limits>

template <typename T,
	bool Supported = (std::is_same_v<std::remove_cv_t<T>, std::wstring> ||
		std::is_floating_point_v<std::remove_cv_t<T>> ||
		std::is_integral_v<std::remove_cv_t<T>>)
>
class DefaultConverter;

// Specialization for supported types
template <typename T>
class DefaultConverter<T, true> {
public:
	static std::wstring ConvertToString(T value) {
		using BaseT = std::remove_cv_t<T>;
		std::fesetround(FE_TONEAREST);

		if constexpr (std::is_same_v<BaseT, std::wstring>) {
			return value;
		}
		else if constexpr (std::is_floating_point_v<BaseT>) {
			return FloatConverter<BaseT>::ConvertToString(value);
		}
		else if constexpr (std::is_integral_v<BaseT>) {
			return std::to_wstring(value);
		}
	}

	static T ConvertFromString(std::wstring text) {
		using BaseT = std::remove_cv_t<T>;
		std::fesetround(FE_TONEAREST);

		if constexpr (std::is_same_v<BaseT, std::wstring>) {
			return text;
		}
		else if constexpr (std::is_floating_point_v<BaseT>) {
			return FloatConverter<BaseT>::ConvertFromString(text);
		}
		else if constexpr (std::is_integral_v<BaseT>) {
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
	}

	static bool IsValidChar(std::wstring text, wchar_t c) {
		using BaseT = std::remove_cv_t<T>;

		if constexpr (std::is_same_v<BaseT, std::wstring>) {
			return c >= 32 && c <= 126;
		}
		else if constexpr (std::is_floating_point_v<BaseT>) {
			return FloatConverter<BaseT>::IsValidChar(text, c);
		}
		else if constexpr (std::is_integral_v<BaseT>) {
			if constexpr (std::is_signed_v<BaseT>) {
				bool hasLength = text.length() > 0;
				if (c == L'-') return !hasLength;
			}

			return (c >= L'0' && c <= L'9');
		}
	}

	static bool AreEqual(T a, T b) {
		using BaseT = std::remove_cv_t<T>;

		if constexpr (std::is_floating_point_v<BaseT>) {
			return FloatConverter<BaseT>::AreEqual(a, b);
		}
		else {
			return a == b;
		}
	}

protected:
	DefaultConverter() = delete;
	~DefaultConverter() = delete;
	DefaultConverter(const DefaultConverter&) = delete;
	DefaultConverter& operator=(const DefaultConverter&) = delete;
};

// Specialization for unsupported types
template <typename T>
class DefaultConverter<T, false> {
public:
	static std::wstring ConvertToString(T value) {
#ifdef DEBUG
			return L"<Unsupported value>";
#else
			return L"";
#endif // DEBUG
	}

protected:
	DefaultConverter() = delete;
	~DefaultConverter() = delete;
	DefaultConverter(const DefaultConverter&) = delete;
	DefaultConverter& operator=(const DefaultConverter&) = delete;
};
