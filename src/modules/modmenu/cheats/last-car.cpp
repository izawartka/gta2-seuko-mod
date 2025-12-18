#include "last-car.h"
#include "../../../events/game-tick.h"
#include "../events/last-car-state.h"
#include "../cheat-registry.h"

ModMenuModule::LastCarCheat* ModMenuModule::LastCarCheat::m_instance = nullptr;

ModMenuModule::LastCarCheat::LastCarCheat(): ModMenuModule::CheatBase("") {
	assert(m_instance == nullptr && "LastCarCheat instance already exists");
	m_instance = this;
}

ModMenuModule::LastCarCheat::~LastCarCheat()
{
	m_instance = nullptr;
}

ModMenuModule::LastCarCheat* ModMenuModule::LastCarCheat::GetInstance()
{
	assert(m_instance && "LastCarCheat not initialized!");
	return m_instance;
}

void ModMenuModule::LastCarCheat::ResetLastCar()
{
	m_lastCar = nullptr;
	m_state = LastCarState::NoCar;
	if (m_watchedCurrentCar != nullptr) {
		m_watchedCurrentCar->RequestUpdate();
	}
}

std::wstring ModMenuModule::LastCarCheat::GetLastCarStateMenuName() const
{
	switch (m_state) {
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
		mem(&Game::Ped::currentCar)
	);

	m_lastCarIdResolver = [this]() -> Game::uint* {
		if (m_lastCar == nullptr) return nullptr;
		return &m_lastCar->id;
	};
}

void ModMenuModule::LastCarCheat::OnEnable()
{
	m_watchedCurrentCar = Core::WatchManager::GetInstance()->Watch<GameTickEvent, Game::Car*, Game::Car*>(
		m_currentCarResolver,
		this,
		&ModMenuModule::LastCarCheat::OnCurrentCarUpdate
	);

	m_watchedLastCarId = Core::WatchManager::GetInstance()->Watch<GameTickEvent, Game::uint>(
		m_lastCarIdResolver,
		this,
		&ModMenuModule::LastCarCheat::OnLastCarIdUpdate
	);
}

void ModMenuModule::LastCarCheat::OnDisable()
{
	Core::WatchManager::GetInstance()->Unwatch(m_watchedCurrentCar);
	m_watchedCurrentCar = nullptr;
	Core::WatchManager::GetInstance()->Unwatch(m_watchedLastCarId);
	m_watchedLastCarId = nullptr;

	m_lastCar = nullptr;
	SetState(LastCarState::NoCar);
}

void ModMenuModule::LastCarCheat::OnCurrentCarUpdate(const std::optional<Game::Car*>& oldValue, const std::optional<Game::Car*>& newValue)
{
	if (newValue.has_value() && newValue.value() != nullptr) {
		m_lastCar = newValue.value();
		m_lastCarId = newValue.value()->id;
		SetState(LastCarState::InCar);
	}
	else {
		// Exited the car
		if (m_lastCar != nullptr) {
			SetState(LastCarState::OutOfCar);
		}

		else {
			SetState(LastCarState::NoCar);
		}
	}
}

void ModMenuModule::LastCarCheat::OnLastCarIdUpdate(const std::optional<Game::uint>& oldValue, const std::optional<Game::uint>& newValue)
{
	if (m_state != LastCarState::OutOfCar) return;
	if (newValue.has_value() && m_lastCarId == newValue.value()) return;

	spdlog::debug("LastCarCheat: Car destroyed or changed, resetting last car");
	m_lastCar = nullptr;
	m_lastCarId = 0;
	SetState(LastCarState::NoCar);
}

void ModMenuModule::LastCarCheat::SetState(LastCarState state)
{
	if (m_state == state) return;
	m_state = state;

	LastCarStateEvent event(state);
	Core::EventManager::GetInstance()->Dispatch(event);
}

REGISTER_CHEAT(LastCarCheat)
