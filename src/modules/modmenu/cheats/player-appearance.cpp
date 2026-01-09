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

	m_watchedPedRemap->SetValue(remap, false);
	if (m_lockedRemap.has_value()) {
		m_lockedRemap = remap;
	}

	DispatchUpdateEvent();
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

void ModMenuModule::PlayerAppearanceCheat::ResetRemap()
{
	if (!IsEnabled()) {
		spdlog::error("PlayerAppearanceCheat::ResetRemap: Cheat is not enabled, cannot reset remap");
		return;
	}

	if (!m_originalRemap.has_value()) {
		spdlog::warn("PlayerAppearanceCheat::ResetRemap: No original remap stored, cannot reset");
		return;
	}

	m_watchedPedRemap->SetValue(m_originalRemap.value(), false);
	if (m_lockedRemap.has_value()) {
		m_lockedRemap = m_originalRemap;
	}

	DispatchUpdateEvent();
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

	m_watchedPedGraphicType->SetValue(graphicType, false);
	if (m_lockedGraphicType.has_value()) {
		m_lockedGraphicType = graphicType;
	}

	DispatchUpdateEvent();
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

void ModMenuModule::PlayerAppearanceCheat::ResetGraphicType()
{
	if (!IsEnabled()) {
		spdlog::error("PlayerAppearanceCheat::ResetGraphicType: Cheat is not enabled, cannot reset graphic type");
		return;
	}
	if (!m_originalGraphicType.has_value()) {
		spdlog::warn("PlayerAppearanceCheat::ResetGraphicType: No original graphic type stored, cannot reset");
		return;
	}
	m_watchedPedGraphicType->SetValue(m_originalGraphicType.value(), false);
	if (m_lockedGraphicType.has_value()) {
		m_lockedGraphicType = m_originalGraphicType;
	}

	DispatchUpdateEvent();
}

void ModMenuModule::PlayerAppearanceCheat::ResetAndDisable()
{
	if (!IsEnabled()) {
		spdlog::error("PlayerAppearanceCheat::ResetAndDisable: Cheat is not enabled, cannot reset and disable");
		return;
	}

	m_lockedRemap = std::nullopt;
	m_lockedGraphicType = std::nullopt;

	ResetRemap();
	ResetGraphicType();
	m_watchedPedRemap->RequestUpdate();
	m_watchedPedGraphicType->RequestUpdate();
	m_isDisabling = true;
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
	m_originalRemap = std::nullopt;
	m_resetAndDisableRemapDone = false;

	watchManager->Unwatch(m_watchedPedGraphicType);
	m_watchedPedGraphicType = nullptr;
	m_originalGraphicType = std::nullopt;
	m_resetAndDisableGraphicTypeDone = false;

	m_isDisabling = false;
}

void ModMenuModule::PlayerAppearanceCheat::OnPedRemapUpdate(const std::optional<Game::PED_REMAP>& oldValue, const std::optional<Game::PED_REMAP>& newValue)
{
	if (m_isDisabling && m_originalRemap.has_value()) {
		if (m_originalRemap.has_value()) {
			m_watchedPedRemap->SetValueNow(m_originalRemap.value(), false);
		}
		m_resetAndDisableRemapDone = true;
		ResetAndDisableCheckAndProceed();
	}

	m_originalRemap = newValue;

	if (m_lockedRemap.has_value()) {
		if (newValue.has_value()) {
			m_watchedPedRemap->SetValueNow(m_lockedRemap.value(), false);
		}
	}
	else {
		DispatchUpdateEvent();
	}
}

void ModMenuModule::PlayerAppearanceCheat::OnPedGraphicTypeUpdate(const std::optional<Game::PED_GRAPHIC_TYPE>& oldValue, const std::optional<Game::PED_GRAPHIC_TYPE>& newValue)
{
	if (m_isDisabling) {
		if (m_originalGraphicType.has_value()) {
			m_watchedPedGraphicType->SetValueNow(m_originalGraphicType.value(), false);
		}
		m_resetAndDisableGraphicTypeDone = true;
		ResetAndDisableCheckAndProceed();
	}

	m_originalGraphicType = newValue;

	if (m_lockedGraphicType.has_value()) {
		if (newValue.has_value()) {
			m_watchedPedGraphicType->SetValueNow(m_lockedGraphicType.value(), false);
		}
	}
	else {
		DispatchUpdateEvent();
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

	if (m_isDisabling) {
		SetEnabled(false);
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

void ModMenuModule::PlayerAppearanceCheat::ResetAndDisableCheckAndProceed()
{
	if(!m_isDisabling || !m_resetAndDisableGraphicTypeDone || !m_resetAndDisableRemapDone) {
		return;
	}

	SetEnabled(false);
}

void ModMenuModule::PlayerAppearanceCheat::DispatchUpdateEvent() const
{
	PlayerAppearanceUpdateEvent event(GetRemap(), GetGraphicType());
	Core::EventManager::GetInstance()->Dispatch(event);
}

REGISTER_CHEAT(PlayerAppearanceCheat)
