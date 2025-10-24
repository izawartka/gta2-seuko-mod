#include "last-car.h"
#include "../../../events/game-tick.h"
#include "../events/last-car-state.h"
#include "../cheat-registry.h"

ModMenuModule::LastCarCheat::LastCarCheat(): ModMenuModule::CheatBase("") {
}

ModMenuModule::LastCarCheat::~LastCarCheat()
{
}

void ModMenuModule::LastCarCheat::ResetLastCar()
{
	m_lastCar = nullptr;
	m_lastState = LastCarState::NoCar;
	if (m_watchedCurrentCar != nullptr) {
		m_watchedCurrentCar->RequestUpdate();
	}
}

std::wstring ModMenuModule::LastCarCheat::GetLastCarStateMenuName() const
{
	switch (m_lastState) {
	case LastCarState::NoCar:
		return L"No last vehicle";
	case LastCarState::InCar:
		return L"Current vehicle";
	case LastCarState::OutOfCar:
		return L"Last vehicle";
	default:
		return L"Unknown State";
	}
}

void ModMenuModule::LastCarCheat::OnFirstEnable()
{
	m_currentCarResolver = Core::MakeResolver(
		Game::Memory::GetPlayerPed,
		mem_addr(&Game::Ped::currentCar)
	);
}

void ModMenuModule::LastCarCheat::OnEnable()
{
	m_watchedCurrentCar = Core::WatchManager::GetInstance()->Watch<GameTickEvent, Game::Car*>(
		m_currentCarResolver,
		this,
		&ModMenuModule::LastCarCheat::OnValueUpdate
	);
}

void ModMenuModule::LastCarCheat::OnDisable()
{
	Core::WatchManager::GetInstance()->Unwatch<Game::Car*>(m_watchedCurrentCar);
	m_watchedCurrentCar = nullptr;
	m_lastCar = nullptr;
	m_lastState = LastCarState::NoCar;
}

void ModMenuModule::LastCarCheat::OnValueUpdate(std::optional<Game::Car*> oldValue, std::optional<Game::Car*> newValue)
{
	if (newValue.has_value() && newValue.value() != nullptr) {
		// Entered a car
		m_lastCar = newValue.value();
		SetState(LastCarState::InCar);
	}
	else {
		// Exited a car
		if (m_lastCar != nullptr) {
			SetState(LastCarState::OutOfCar);
		}
		else {
			SetState(LastCarState::NoCar);
		}
	}
}

void ModMenuModule::LastCarCheat::SetState(LastCarState state)
{
	if (m_lastState == state) return;
	m_lastState = state;

	LastCarStateEvent event(state);
	Core::EventManager::GetInstance()->Dispatch(event);
}

REGISTER_CHEAT(LastCarCheat)
