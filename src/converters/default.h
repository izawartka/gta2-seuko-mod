#pragma once
#include "float.h"
#include "integer.h"
#include <limits>
#include <type_traits>

template <typename T,
	bool Supported = (std::is_same_v<std::remove_cv_t<T>, std::wstring> ||
		std::is_floating_point_v<std::remove_cv_t<T>> ||
		std::is_integral_v<std::remove_cv_t<T>> ||
		std::is_enum_v<std::remove_cv_t<T>>)
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
		else if constexpr (std::is_enum_v<BaseT>) {
			using UnderlyingT = std::underlying_type_t<BaseT>;
			return IntegerConverter<UnderlyingT>::ConvertToString(static_cast<UnderlyingT>(value));
		}
		else if constexpr (std::is_floating_point_v<BaseT>) {
			return FloatConverter<BaseT>::ConvertToString(value);
		}
		else if constexpr (std::is_integral_v<BaseT>) {
			return IntegerConverter<BaseT>::ConvertToString(value);
		}
	}

	static T ConvertFromString(std::wstring text) {
		using BaseT = std::remove_cv_t<T>;
		std::fesetround(FE_TONEAREST);

		if constexpr (std::is_same_v<BaseT, std::wstring>) {
			return text;
		}
		else if constexpr (std::is_enum_v<BaseT>) {
			using UnderlyingT = std::underlying_type_t<BaseT>;
			UnderlyingT value = IntegerConverter<UnderlyingT>::ConvertFromString(text);
			return static_cast<BaseT>(value);
		}
		else if constexpr (std::is_floating_point_v<BaseT>) {
			return FloatConverter<BaseT>::ConvertFromString(text);
		}
		else if constexpr (std::is_integral_v<BaseT>) {
			return IntegerConverter<BaseT>::ConvertFromString(text);
		}
	}

	static bool IsValidChar(std::wstring text, wchar_t c) {
		using BaseT = std::remove_cv_t<T>;

		if constexpr (std::is_same_v<BaseT, std::wstring>) {
			return c >= 32 && c <= 126;
		}
		else if constexpr (std::is_enum_v<BaseT>) {
			using UnderlyingT = std::underlying_type_t<BaseT>;
			return IntegerConverter<UnderlyingT>::IsValidChar(text, c);
		}
		else if constexpr (std::is_floating_point_v<BaseT>) {
			return FloatConverter<BaseT>::IsValidChar(text, c);
		}
		else if constexpr (std::is_integral_v<BaseT>) {
			return IntegerConverter<BaseT>::IsValidChar(text, c);
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
		return L"<Unsupported value>";
	}

protected:
	DefaultConverter() = delete;
	~DefaultConverter() = delete;
	DefaultConverter(const DefaultConverter&) = delete;
	DefaultConverter& operator=(const DefaultConverter&) = delete;
};
