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
            static_assert(std::is_same<decltype(ConverterT::ConvertToString(std::declval<T>())), std::wstring>::value,
                "ConverterT must have a static method: std::wstring ConvertToString(T)");
            static_assert(std::is_same<decltype(ConverterT::ConvertFromString(std::declval<std::wstring>())), T>::value,
                "ConverterT must have a static method: T ConvertFromString(std::wstring)");
            static_assert(std::is_same<decltype(ConverterT::IsValidChar(std::declval<std::wstring>(), std::declval<wchar_t>())), bool>::value,
                "ConverterT must have a static method: bool IsValidChar(std::wstring, wchar_t)");

            m_toStringFunc = ConverterT::ConvertToString;
            m_fromStringFunc = ConverterT::ConvertFromString;
            m_isValidCharFunc = ConverterT::IsValidChar;
        }

        void ClearConverter() {
            m_toStringFunc = DefaultConverter<T>::ConvertToString;
            m_fromStringFunc = DefaultConverter<T>::ConvertFromString;
            m_isValidCharFunc = DefaultConverter<T>::IsValidChar;
        }

    protected:
        std::wstring ConvertToString(T value) const {
			assert(m_toStringFunc && "Converter not set");
            return m_toStringFunc(value);
        }

        T ConvertFromString(std::wstring value) const {
			assert(m_fromStringFunc && "Converter not set");
            return m_fromStringFunc(value);
        }

        bool IsValidChar(std::wstring text, wchar_t c) const {
			assert(m_isValidCharFunc && "Converter not set");
            return m_isValidCharFunc(text, c);
        }

        ConverterSupport() = default;

    private:
        std::function<std::wstring(T)> m_toStringFunc = DefaultConverter<T>::ConvertToString;
        std::function<T(std::wstring)> m_fromStringFunc = DefaultConverter<T>::ConvertFromString;
        std::function<bool(std::wstring, wchar_t)> m_isValidCharFunc = DefaultConverter<T>::IsValidChar;
    };
}
