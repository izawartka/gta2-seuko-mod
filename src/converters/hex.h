#pragma once
#include <string>
#include <sstream>
#include <cfenv>
#include <type_traits>
#include <limits>
#include <algorithm>

template <typename T,
	bool Supported = (std::is_integral_v<std::remove_cv_t<T>> || std::is_enum_v<std::remove_cv_t<T>>)
>
class HexConverter;

// Specialization for supported types
template <typename T>
class HexConverter<T, true> {
public:
	static std::wstring ConvertToString(T value) {
		using BaseT = std::remove_cv_t<T>;
		std::fesetround(FE_TONEAREST);

		if constexpr (std::is_enum_v<BaseT>) {
			using UnderlyingT = std::underlying_type_t<BaseT>;
			UnderlyingT val = static_cast<UnderlyingT>(value);
			return HexConverter<UnderlyingT>::ConvertToString(val);
		}
		else {
			std::wstringstream wss;
			if constexpr (std::is_signed_v<BaseT>) {
				using UBaseT = std::make_unsigned_t<BaseT>;
				if (value < 0) {
					wss << L'-';
					UBaseT uval = static_cast<UBaseT>(-value);
					wss << std::hex << uval;
				}
				else {
					wss << std::hex << static_cast<UBaseT>(value);
				}
			}
			else {
				wss << std::hex << value;
			}
			return wss.str();
		}
	}

	static T ConvertFromString(std::wstring text) {
		using BaseT = std::remove_cv_t<T>;
		std::fesetround(FE_TONEAREST);

		if (text.empty()) return static_cast<BaseT>(0);

		if constexpr (std::is_enum_v<BaseT>) {
			using UnderlyingT = std::underlying_type_t<BaseT>;
			UnderlyingT val = HexConverter<UnderlyingT>::ConvertFromString(text);
			return static_cast<BaseT>(val);
		}
		else {
			using longT = std::conditional_t<std::is_signed_v<BaseT>, long long, unsigned long long>;
			longT value = 0;
			if constexpr (std::is_signed_v<BaseT>) {
				value = std::stoll(text, nullptr, 16);
			}
			else {
				value = std::stoull(text, nullptr, 16);
			}
			value = std::min(value, static_cast<longT>(std::numeric_limits<BaseT>::max()));
			value = std::max(value, static_cast<longT>(std::numeric_limits<BaseT>::min()));
			return static_cast<BaseT>(value);
		}
	}

	static bool IsValidChar(std::wstring text, wchar_t c) {
		using BaseT = std::remove_cv_t<T>;

		if constexpr (std::is_signed_v<BaseT>) {
			bool hasLength = text.length() > 0;
			if (c == L'-') return !hasLength;
		}
			
		return (c >= L'0' && c <= L'9') ||
			(c >= L'a' && c <= L'f') ||
			(c >= L'A' && c <= L'F');
	}

protected:
	HexConverter() = delete;
	~HexConverter() = delete;
	HexConverter(const HexConverter&) = delete;
	HexConverter& operator=(const HexConverter&) = delete;
};

// Specialization for unsupported types
template <typename T>
class HexConverter<T, false> {
public:
	static std::wstring ConvertToString(T value) {
		return L"<Unsupported value>";
	}

protected:
	HexConverter() = delete;
	~HexConverter() = delete;
	HexConverter(const HexConverter&) = delete;
	HexConverter& operator=(const HexConverter&) = delete;
};