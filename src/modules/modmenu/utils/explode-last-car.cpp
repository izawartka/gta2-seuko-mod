#include "explode-last-car.h"
#include "../cheats/last-car.h"
#include "../toast-manager.h"

ModMenuModule::Utils::ExplodeLastCarResult ModMenuModule::Utils::ExplodeLastCar(Game::EXPLOSION_SIZE explosionSize)
{
	LastCarCheat* lastCarCheat = LastCarCheat::GetInstance();
	if (!lastCarCheat->IsEnabled()) {
		return ExplodeLastCarResult::NoLastCar;
	}

	Game::Car* car = lastCarCheat->GetLastCar();
	if (!car) {
		return ExplodeLastCarResult::NoLastCar;
	}

	if (car->carDamage >= 32001) {
		return ExplodeLastCarResult::AlreadyExploded;
	}

	Game::Functions::ExplodeCar(car, 0, explosionSize);
	return ExplodeLastCarResult::Success;
}

ModMenuModule::Utils::ExplodeLastCarResult ModMenuModule::Utils::ExplodeLastCarWithToast(Game::EXPLOSION_SIZE explosionSize, bool showSuccessToast)
{
	ExplodeLastCarResult result = ExplodeLastCar(explosionSize);

	switch (result) {
	case ExplodeLastCarResult::Success:
		spdlog::info("ExplodeLastCarWithToast: Exploded last vehicle");
		if (showSuccessToast) ToastManager::GetInstance()->Show({ L"Exploded last vehicle", ToastType::Info });
		break;
	case ExplodeLastCarResult::AlreadyExploded:
		spdlog::warn("ExplodeLastCarWithToast: Last vehicle is already exploded");
		ToastManager::GetInstance()->Show({ L"Last vehicle is already exploded", ToastType::Warning });
		break;
	case ExplodeLastCarResult::NoLastCar:
		spdlog::warn("ExplodeLastCarWithToast: No last vehicle to explode");
		ToastManager::GetInstance()->Show({ L"No last vehicle to explode", ToastType::Warning });
		break;
	default:
		spdlog::error("ExplodeLastCarWithToast: Unknown error while exploding last vehicle");
		ToastManager::GetInstance()->Show({ L"Unknown error while exploding last vehicle", ToastType::Error });
		break;
	}

	return result;
}
