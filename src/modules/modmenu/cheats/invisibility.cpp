#include "invisibility.h"
#include "../../../events/game-tick.h"
#include "../cheat-registry.h"

ModMenuModule::InvisibilityCheat* ModMenuModule::InvisibilityCheat::m_instance = nullptr;

ModMenuModule::InvisibilityCheat::InvisibilityCheat() : ModMenuModule::CheatBase("Cheat_Invisibility_IsEnabled") {
	assert(m_instance == nullptr && "InvisibilityCheat instance already exists");
	m_instance = this;
}

ModMenuModule::InvisibilityCheat::~InvisibilityCheat()
{
	m_instance = nullptr;
}

ModMenuModule::InvisibilityCheat* ModMenuModule::InvisibilityCheat::GetInstance()
{
	assert(m_instance && "InvisibilityCheat not initialized!");
	return m_instance;
}

void ModMenuModule::InvisibilityCheat::ResetAndDisable()
{
	if (!IsEnabled()) {
		spdlog::error("InvisibilityCheat::ResetAndDisable: Cheat is not enabled, cannot reset and disable");
		return;
	}

	if (m_watchedBitstate) {
		m_watchedBitstate->RequestUpdate();
		m_isDisabling = true;
	}
	else {
		SetEnabled(false);
	}
}

void ModMenuModule::InvisibilityCheat::OnFirstEnable()
{
	m_bitstateResolver = Core::MakeResolver(
		Game::Memory::GetPlayerPed,
		mem(&Game::Ped::bitState)
	);

	m_invisibilityResolver = Core::MakeResolver(
		Game::Memory::GetPlayerPed,
		mem(&Game::Ped::gameObject),
		mem(&Game::GameObject::sprite),
		mem(&Game::Sprite::invisibility)
	);
}

void ModMenuModule::InvisibilityCheat::OnEnable()
{
	Core::WatchManager* watchManager = Core::WatchManager::GetInstance();

	m_watchedBitstate = watchManager->Watch<GameTickEvent>(
		m_bitstateResolver,
		this,
		&ModMenuModule::InvisibilityCheat::OnBitstateUpdate
	);

	m_watchedInvisibility = watchManager->Watch< GameTickEvent>(
		m_invisibilityResolver,
		this,
		&ModMenuModule::InvisibilityCheat::OnInvisibilityUpdate
	);

	AddEventListener<GameEndEvent>(&ModMenuModule::InvisibilityCheat::OnGameEnd);
}

void ModMenuModule::InvisibilityCheat::OnDisable()
{
	RemoveEventListener<GameEndEvent>();

	Core::WatchManager* watchManager = Core::WatchManager::GetInstance();

	watchManager->Unwatch(m_watchedBitstate);
	m_watchedBitstate = nullptr;
	watchManager->Unwatch(m_watchedInvisibility);
	m_watchedInvisibility = nullptr;
	m_isDisabling = false;
}

void ModMenuModule::InvisibilityCheat::OnBitstateUpdate(const std::optional<Game::PED_BIT_STATE>& oldValue, const std::optional<Game::PED_BIT_STATE>& newValue)
{
	// ResetAndDisable is managed here because OnInvisibilityUpdate is not guaranteed to be called (player may be in a car)
	if (m_isDisabling) {
		Game::PED_BIT_STATE newBitstate = static_cast<Game::PED_BIT_STATE>(newValue.value() & ~Game::PED_BIT_STATE_INVISIBLE);
		m_watchedBitstate->SetValueNow(newBitstate, false);
		m_watchedInvisibility->SetValueNow(Game::SPRITE_INVISIBILITY_VISIBLE, false);
		SetEnabled(false);
		return;
	}

	if (newValue.has_value()) {
		Game::PED_BIT_STATE newBitstate = static_cast<Game::PED_BIT_STATE>(newValue.value() | Game::PED_BIT_STATE_INVISIBLE);
		m_watchedBitstate->SetValueNow(newBitstate, false);
	}
}

void ModMenuModule::InvisibilityCheat::OnInvisibilityUpdate(const std::optional<Game::SPRITE_INVISIBILITY>& oldValue, const std::optional<Game::SPRITE_INVISIBILITY>& newValue)
{
	if (!m_isDisabling && newValue.has_value()) {
		m_watchedInvisibility->SetValueNow(Game::SPRITE_INVISIBILITY_GHOST, false);
	}
}

void ModMenuModule::InvisibilityCheat::OnGameEnd(GameEndEvent& event)
{
	if (m_isDisabling) {
		SetEnabled(false);
	}
}

REGISTER_CHEAT(InvisibilityCheat)
