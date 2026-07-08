#pragma once
#include <string>
#include "car-model.h"
#include "../game/game.h"

class CarModelNameConverter {
public:
	static std::wstring ConvertToString(Game::CAR_MODEL4 value) {
		static Game::Car dummyCar;
		dummyCar.carModel = value; // not the prettiest way to do it, but the easiest:)
		wchar_t* modelName = Game::Functions::GetCarModelName(&dummyCar);
		if (modelName == nullptr) return CarModelConverter::ConvertToString(value);
		return std::wstring(modelName);
	}

	static bool AreEqual(Game::CAR_MODEL4 a, Game::CAR_MODEL4 b) {
		return a == b;
	}

protected:
	CarModelNameConverter() = delete;
	~CarModelNameConverter() = delete;
	CarModelNameConverter(const CarModelNameConverter&) = delete;
	CarModelNameConverter& operator=(const CarModelNameConverter&) = delete;
};
