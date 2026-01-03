#include "player-appearance.h"
#include "../../../events/game-tick.h"
#include "../events/player-appearance-update.h"
#include "../cheat-registry.h"

ModMenuModule::PlayerAppearanceCheat* ModMenuModule::PlayerAppearanceCheat::m_instance = nullptr;

ModMenuModule::PlayerAppearanceCheat::PlayerAppearanceCheat() : ModMenuModule::CheatBase("Cheat_PlayerAppearance_IsEnabled") {
	assert(m_instance == nullptr && "PlayerAppearanceCheat instance already exists");
	m_instance = this;
}

ModMenuModule::PlayerAppearanceCheat::~PlayerAppearanceCheat()
{
	m_instance = nullptr;
}

ModMenuModule::PlayerAppearanceCheat* ModMenuModule::PlayerAppearanceCheat::GetInstance()
{
	assert(m_instance && "PlayerAppearanceCheat not initialized!");
	return m_instance;
}

void ModMenuModule::PlayerAppearanceCheat::SetRemapLocked(bool locked)
{
	if (!IsEnabled()) {
		spdlog::error("PlayerAppearanceCheat::SetRemapLocked: Cheat is not enabled, cannot set remap lock");
		return;
	}

	m_lockedRemap = locked ? GetRemap() : std::nullopt;
}

bool ModMenuModule::PlayerAppearanceCheat::IsRemapLocked() const
{
	return IsEnabled() && m_lockedRemap.has_value();
}

void ModMenuModule::PlayerAppearanceCheat::SetRemap(Game::PED_REMAP remap)
{
	if (!IsEnabled()) {
		spdlog::error("PlayerAppearanceCheat::SetRemap: Cheat is not enabled, cannot set remap");
		return;
	}

	m_watchedPedRemap->SetValue(remap, true);
	if (m_lockedRemap.has_value()) {
		m_lockedRemap = remap;
	}
}

std::optional<Game::PED_REMAP> ModMenuModule::PlayerAppearanceCheat::GetRemap() const
{
	if (!IsEnabled()) {
		spdlog::error("PlayerAppearanceCheat::GetRemap: Cheat is not enabled, cannot get remap");
		return std::nullopt;
	}

	if (m_watchedPedRemap->GetNextValue().has_value()) {
		return m_watchedPedRemap->GetNextValue();
	}

	return m_watchedPedRemap->GetSavedValue();
}

void ModMenuModule::PlayerAppearanceCheat::SetGraphicTypeLocked(bool locked)
{
	if (!IsEnabled()) {
		spdlog::error("PlayerAppearanceCheat::SetGraphicTypeLocked: Cheat is not enabled, cannot set graphic type lock");
		return;
	}

	m_lockedGraphicType = locked ? GetGraphicType() : std::nullopt;
}

bool ModMenuModule::PlayerAppearanceCheat::IsGraphicTypeLocked() const
{
	return IsEnabled() && m_lockedGraphicType.has_value();
}

void ModMenuModule::PlayerAppearanceCheat::SetGraphicType(Game::PED_GRAPHIC_TYPE graphicType)
{
	if (!IsEnabled()) {
		spdlog::error("PlayerAppearanceCheat::SetGraphicType: Cheat is not enabled, cannot set graphic type");
		return;
	}

	m_watchedPedGraphicType->SetValue(graphicType, true);
	if (m_lockedGraphicType.has_value()) {
		m_lockedGraphicType = graphicType;
	}
}

std::optional<Game::PED_GRAPHIC_TYPE> ModMenuModule::PlayerAppearanceCheat::GetGraphicType() const
{
	if (!IsEnabled()) {
		spdlog::error("PlayerAppearanceCheat::GetGraphicType: Cheat is not enabled, cannot get graphic type");
		return std::nullopt;
	}

	if (m_watchedPedGraphicType->GetNextValue().has_value()) {
		return m_watchedPedGraphicType->GetNextValue();
	}

	return m_watchedPedGraphicType->GetSavedValue();
}

void ModMenuModule::PlayerAppearanceCheat::OnFirstEnable()
{
	LoadFromPersistence();
	m_pedRemapResolver = Core::MakeResolver(Game::Memory::GetPlayerPed, mem(&Game::Ped::remap));
	m_pedGraphicTypeResolver = Core::MakeResolver(Game::Memory::GetPlayerPed, mem(&Game::Ped::graphicType));
}

void ModMenuModule::PlayerAppearanceCheat::OnEnable()
{
	Core::WatchManager* watchManager = Core::WatchManager::GetInstance();

	m_watchedPedRemap = watchManager->Watch<GameTickEvent>(
		m_pedRemapResolver,
		this,
		&ModMenuModule::PlayerAppearanceCheat::OnPedRemapUpdate
	);

	m_watchedPedGraphicType = watchManager->Watch<GameTickEvent>(
		m_pedGraphicTypeResolver,
		this,
		&ModMenuModule::PlayerAppearanceCheat::OnPedGraphicTypeUpdate
	);

	AddEventListener<GameEndEvent>(&ModMenuModule::PlayerAppearanceCheat::OnGameEnd);
}

void ModMenuModule::PlayerAppearanceCheat::OnDisable()
{
	RemoveEventListener<GameEndEvent>();

	SaveToPersistence();
	Core::WatchManager* watchManager = Core::WatchManager::GetInstance();

	watchManager->Unwatch(m_watchedPedRemap);
	m_watchedPedRemap = nullptr;

	watchManager->Unwatch(m_watchedPedGraphicType);
	m_watchedPedGraphicType = nullptr;
}

void ModMenuModule::PlayerAppearanceCheat::OnPedRemapUpdate(const std::optional<Game::PED_REMAP>& oldValue, const std::optional<Game::PED_REMAP>& newValue)
{
	if (m_lockedRemap.has_value()) {
		if (newValue.has_value()) {
			m_watchedPedRemap->SetValueNow(m_lockedRemap.value(), false);
		}
	}
	else {
		PlayerAppearanceUpdateEvent event(newValue, m_watchedPedGraphicType->GetSavedValue());
		Core::EventManager::GetInstance()->Dispatch(event);
	}
}

void ModMenuModule::PlayerAppearanceCheat::OnPedGraphicTypeUpdate(const std::optional<Game::PED_GRAPHIC_TYPE>& oldValue, const std::optional<Game::PED_GRAPHIC_TYPE>& newValue)
{
	if (m_lockedGraphicType.has_value()) {
		if (newValue.has_value()) {
			m_watchedPedGraphicType->SetValueNow(m_lockedGraphicType.value(), false);
		}
	}
	else {
		PlayerAppearanceUpdateEvent event(m_watchedPedRemap->GetSavedValue(), newValue);
		Core::EventManager::GetInstance()->Dispatch(event);
	}
}

void ModMenuModule::PlayerAppearanceCheat::OnGameEnd(GameEndEvent& event)
{
	if (!m_lockedRemap.has_value()) {
		m_watchedPedRemap->CancelSetValue();
	}
	if (!m_lockedGraphicType.has_value()) {
		m_watchedPedGraphicType->CancelSetValue();
	}
}

void ModMenuModule::PlayerAppearanceCheat::SaveToPersistence() const
{
	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();
	persistence->Save("Cheat_PlayerAppearance_LockedRemap", m_lockedRemap);
	persistence->Save("Cheat_PlayerAppearance_LockedGraphicType", m_lockedGraphicType);
}

void ModMenuModule::PlayerAppearanceCheat::LoadFromPersistence()
{
	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();
	m_lockedRemap = persistence->Load<std::optional<Game::PED_REMAP>>("Cheat_PlayerAppearance_LockedRemap", std::nullopt);
	m_lockedGraphicType = persistence->Load<std::optional<Game::PED_GRAPHIC_TYPE>>("Cheat_PlayerAppearance_LockedGraphicType", std::nullopt);
}

REGISTER_CHEAT(PlayerAppearanceCheat)
