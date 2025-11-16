#pragma once
#include "common.h"
#include "../../converters/default.h"

namespace UiModule {
	template<typename T>
	class ConverterSupport {
	public:
		virtual ~ConverterSupport() = default;

		template<typename ConverterT>
		void SetConverter() {
			if constexpr (has_convert_to_string<ConverterT, T>::value) {
				m_toStringFunc = ConverterT::ConvertToString;
			} 
			else {
				m_toStringFunc = nullptr;
			}

			if constexpr (has_convert_from_string<ConverterT, T>::value) {
				m_fromStringFunc = ConverterT::ConvertFromString;
			} 
			else {
				m_fromStringFunc = nullptr;
			}

			if constexpr (has_is_valid_char<ConverterT, T>::value) {
				m_isValidCharFunc = ConverterT::IsValidChar;
			}
			else {
				m_isValidCharFunc = nullptr;
			}

			if constexpr (has_are_equal<ConverterT, T>::value) {
				m_areEqual = ConverterT::AreEqual;
			}
			else {
				m_areEqual = nullptr;
			}

			OnConverterChanged();
		}

		void ClearConverter() {
			// Reset to defaults according to availability on DefaultConverter<T>
			if constexpr (has_convert_to_string<DefaultConverter<T>, T>::value) {
				m_toStringFunc = DefaultConverter<T>::ConvertToString;
			} else {
				m_toStringFunc = nullptr;
			}

			if constexpr (has_convert_from_string<DefaultConverter<T>, T>::value) {
				m_fromStringFunc = DefaultConverter<T>::ConvertFromString;
			} else {
				m_fromStringFunc = nullptr;
			}

			if constexpr (has_is_valid_char<DefaultConverter<T>, T>::value) {
				m_isValidCharFunc = DefaultConverter<T>::IsValidChar;
			} else {
				m_isValidCharFunc = nullptr;
			}

			if constexpr (has_are_equal<DefaultConverter<T>, T>::value) {
				m_areEqual = DefaultConverter<T>::AreEqual;
			} else {
				m_areEqual = nullptr;
			}

			OnConverterChanged();
		}

	protected:
		std::wstring ConvertToString(T value) const {
			assert(m_toStringFunc && "Converter not set or does not support to-string conversion");
			return m_toStringFunc(value);
		}

		T ConvertFromString(std::wstring value) const {
			assert(m_fromStringFunc && "Converter not set or does not support from-string conversion");
			return m_fromStringFunc(value);
		}

		bool IsValidChar(std::wstring text, wchar_t c) const {
			assert(m_isValidCharFunc && "Converter not set or does not support char validation");
			return m_isValidCharFunc(text, c);
		}

		bool AreEqual(T a, T b) const {
			assert(m_areEqual && "Converter not set or does not support equality comparison");
			return m_areEqual(a, b);
		}

		virtual void OnConverterChanged() {}

		// Explicit constructor to initialize default converters conditionally
		ConverterSupport() {
			ClearConverter();
		}

	private:
		template<typename ConverterT, typename U, typename = void>
		struct has_convert_to_string : std::false_type {};
		template<typename ConverterT, typename U>
		struct has_convert_to_string<ConverterT, U, std::void_t<decltype(ConverterT::ConvertToString(std::declval<U>()))>> : std::true_type {};

		template<typename ConverterT, typename U, typename = void>
		struct has_convert_from_string : std::false_type {};
		template<typename ConverterT, typename U>
		struct has_convert_from_string<ConverterT, U, std::void_t<decltype(ConverterT::ConvertFromString(std::declval<std::wstring>()))>> : std::true_type {};

		template<typename ConverterT, typename U, typename = void>
		struct has_is_valid_char : std::false_type {};
		template<typename ConverterT, typename U>
		struct has_is_valid_char<ConverterT, U, std::void_t<decltype(ConverterT::IsValidChar(std::declval<std::wstring>(), std::declval<wchar_t>()))>> : std::true_type {};

		template<typename ConverterT, typename U, typename = void>
		struct has_are_equal : std::false_type {};
		template<typename ConverterT, typename U>
		struct has_are_equal<ConverterT, U, std::void_t<decltype(ConverterT::AreEqual(std::declval<U>(), std::declval<U>()))>> : std::true_type {};

		std::function<std::wstring(T)> m_toStringFunc;
		std::function<T(std::wstring)> m_fromStringFunc;
		std::function<bool(std::wstring, wchar_t)> m_isValidCharFunc;
		std::function<bool(T, T)> m_areEqual;
	};
}
