#pragma once
#include <string>

class GtaFontSafeConverter {
public:
	static std::wstring ConvertToString(std::wstring value) {
		return value;
	}

	static std::wstring ConvertFromString(std::wstring text) {
		return text;
	}

	static bool IsValidChar(std::wstring text, wchar_t c) {
		return ((c >= L'a' && c <= L'z') ||
			(c >= L'A' && c <= L'Z') ||
			(c >= L'0' && c <= L'9') ||
			c == L'!' || c == L'"' || c == L'$' || c == L'%' || c == L'\'' ||
			c == L'(' || c == L')' || c == L'*' || c == L',' || c == L'-' ||
			c == L'.' || c == L'/' || c == L':' || c == L';' || c == L'?' ||
			c == L'`' || c == L'~' || c == L' ');
	}

	static bool AreEqual(std::wstring a, std::wstring b) {
		return a == b;
	}

protected:
	GtaFontSafeConverter() = delete;
	~GtaFontSafeConverter() = delete;
	GtaFontSafeConverter(const GtaFontSafeConverter&) = delete;
	GtaFontSafeConverter& operator=(const GtaFontSafeConverter&) = delete;
};
