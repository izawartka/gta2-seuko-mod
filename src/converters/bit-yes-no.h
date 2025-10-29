#pragma once
#include "yes-no.h"
#include <string>

template <typename T, T bitmask, bool requireAllBits = false>
class BitYesNoConverter {
public:
	static std::wstring ConvertToString(T value) {
		bool boolValue = GetBoolValue(value);
		return YesNoConverter::ConvertToString(boolValue);
	}

	static bool AreEqual(T a, T b) {
		bool boolA = GetBoolValue(a);
		bool boolB = GetBoolValue(b);
		return YesNoConverter::AreEqual(boolA, boolB);
	}

protected:
	static bool GetBoolValue(T value) {
		if constexpr (requireAllBits) {
			return (value & bitmask) == bitmask;
		}
		else {
			return (value & bitmask) != 0;
		}
	}

	BitYesNoConverter() = delete;
	~BitYesNoConverter() = delete;
	BitYesNoConverter(const BitYesNoConverter&) = delete;
	BitYesNoConverter& operator=(const BitYesNoConverter&) = delete;
};
