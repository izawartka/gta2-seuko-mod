#pragma once
#include <string>

template <typename T, typename ConverterT>
class InBracketsConverter {
public:
	static std::wstring ConvertToString(T value) {
		std::wstring inner = ConverterT::ConvertToString(value);
		if (inner.empty()) {
			return L"";
		}

		return L" (" + inner + L")";
	}

	static bool AreEqual(T a, T b) {
		return ConverterT::AreEqual(a, b);
	}

protected:
	InBracketsConverter() = delete;
	~InBracketsConverter() = delete;
	InBracketsConverter(const InBracketsConverter&) = delete;
	InBracketsConverter& operator=(const InBracketsConverter&) = delete;
};
