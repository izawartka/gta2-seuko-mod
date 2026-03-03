#pragma once
#include <string>

template <typename T, typename ConverterT, size_t maxChars = 20>
class ElipsisConverter {
public:
	static std::wstring ConvertToString(T value) {
		std::wstring text = ConverterT::ConvertToString(value);
		if (text.size() < maxChars) {
			return text;
		}

		return text.substr(0, maxChars - 3) + L"...";
	}

	static bool AreEqual(T a, T b) {
		return ConverterT::AreEqual(a, b);
	}

protected:
	ElipsisConverter() = delete;
	~ElipsisConverter() = delete;
	ElipsisConverter(const ElipsisConverter&) = delete;
	ElipsisConverter& operator=(const ElipsisConverter&) = delete;
};
