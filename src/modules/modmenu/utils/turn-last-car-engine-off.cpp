#include "turn-last-car-engine-off.h"
#include "../cheats/last-car.h"
#include "../toast-manager.h"

ModMenuModule::Utils::TurnLastCarEngineOffResult ModMenuModule::Utils::TurnLastCarEngineOff()
{
	LastCarCheat* lastCarCheat = LastCarCheat::GetInstance();
	if (!lastCarCheat->IsEnabled()) {
		return TurnLastCarEngineOffResult::NoLastCar;
	}

	Game::Car* car = lastCarCheat->GetLastCar();
	if (!car) {
		return TurnLastCarEngineOffResult::NoLastCar;
	}

	if (car->engineState != Game::CAR_ENGINE_STATE_ON) {
		return TurnLastCarEngineOffResult::AlreadyOff;
	}

	car->engineState = Game::CAR_ENGINE_STATE_TURNING_OFF;
	return TurnLastCarEngineOffResult::Success;
}

ModMenuModule::Utils::TurnLastCarEngineOffResult ModMenuModule::Utils::TurnLastCarEngineOffWithToast(bool showSuccessToast)
{
	TurnLastCarEngineOffResult result = TurnLastCarEngineOff();

	switch (result) {
	case TurnLastCarEngineOffResult::Success:
		spdlog::info("TurnLastCarEngineOffWithToast: Turned off last vehicle engine");
		if (showSuccessToast) ToastManager::GetInstance()->Show({ L"Turned off last vehicle engine", ToastType::Info });
		break;
	case TurnLastCarEngineOffResult::AlreadyOff:
		spdlog::warn("TurnLastCarEngineOffWithToast: Last vehicle engine is already off");
		ToastManager::GetInstance()->Show({ L"Last vehicle engine is already off", ToastType::Warning });
		break;
	case TurnLastCarEngineOffResult::NoLastCar:
		spdlog::warn("TurnLastCarEngineOffWithToast: No last vehicle to turn engine off");
		ToastManager::GetInstance()->Show({ L"No last vehicle to turn engine off", ToastType::Warning });
		break;
	default:
		spdlog::error("TurnLastCarEngineOffWithToast: Unknown error while turning off last vehicle engine");
		ToastManager::GetInstance()->Show({ L"Unknown error while turning off last vehicle engine", ToastType::Error });
		break;
	};

	return result;
}
