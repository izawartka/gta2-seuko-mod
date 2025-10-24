#pragma once
#include <string>
#include <cfenv>
#include <type_traits>
#include <cassert>
#include <limits>

template <typename T>
class DefaultConverter {
public:
	static std::wstring ConvertToString(T value) {
		using BaseT = std::remove_cv_t<T>;
		std::fesetround(FE_TONEAREST);

		if constexpr (std::is_same_v<BaseT, std::wstring>) {
			return value;
		}
		else if constexpr (std::is_floating_point_v<BaseT>) {
			return std::to_wstring(value);
		}
		else if constexpr (std::is_integral_v<BaseT>) {
			return std::to_wstring(value);
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

		if constexpr (std::is_same_v<BaseT, std::wstring>) {
			return text;
		}
		else if constexpr (std::is_floating_point_v<BaseT>) {
			return static_cast<BaseT>(std::stod(text));
		}
		else if constexpr (std::is_integral_v<BaseT>) {
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
			if (c == L'-') return !hasLength;

			return (c >= L'0' && c <= L'9');
		}
		else if constexpr (std::is_integral_v<BaseT>) {
			if constexpr (std::is_signed_v<BaseT>) {
				bool hasLength = text.length() > 0;
				if (c == L'-') return !hasLength;
			}

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
