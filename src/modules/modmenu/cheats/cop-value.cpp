#include "cop-value.h"
#include "../../../events/game-tick.h"
#include "../cheat-registry.h"

ModMenuModule::CopValueCheat* ModMenuModule::CopValueCheat::m_instance = nullptr;

ModMenuModule::CopValueCheat::CopValueCheat()
	: ModMenuModule::CheatBase("Cheat_CopValue_IsEnabled") {
	assert(m_instance == nullptr && "CopValueCheat instance already exists");
	m_instance = this;
}

ModMenuModule::CopValueCheat::~CopValueCheat()
{
	m_instance = nullptr;
}

ModMenuModule::CopValueCheat* ModMenuModule::CopValueCheat::GetInstance()
{
	assert(m_instance && "CopValueCheat not initialized!");
	return m_instance;
}

void ModMenuModule::CopValueCheat::SetCopValueLocked(bool locked)
{
	if (!IsEnabled()) {
		spdlog::error("CopValueCheat::SetCopValueLocked: Cheat is not enabled, cannot set cop value lock");
		return;
	}

	m_lockedCopValue = locked ? GetCopValue() : std::nullopt;
	UpdateCopValueChangeListener();
}

bool ModMenuModule::CopValueCheat::IsCopValueLocked() const
{
	return IsEnabled() && m_lockedCopValue.has_value();
}

void ModMenuModule::CopValueCheat::SetCopValue(short copValue)
{
	if (!IsEnabled()) {
		spdlog::error("CopValueCheat::SetCopValue: Cheat is not enabled, cannot set cop value");
		return;
	}
	m_watchedCopValue->SetValue(copValue);
	if (m_lockedCopValue.has_value()) {
		m_lockedCopValue = copValue;
	}
}

const std::optional<short>& ModMenuModule::CopValueCheat::GetCopValue() const
{
	static const std::optional<short> emptyValue = std::nullopt;
	if (!IsEnabled()) {
		spdlog::error("CopValueCheat::GetCopValue: Cheat is not enabled, cannot get cop value");
		return emptyValue;
	}

	const auto& nextValue = m_watchedCopValue->GetNextValue();
	if (nextValue.has_value()) {
		return nextValue;
	}

	return m_watchedCopValue->GetSavedValue();
}

void ModMenuModule::CopValueCheat::OnFirstEnable()
{
	LoadFromPersistence();

	m_copValueResolver = Core::MakeResolver(
		Game::Memory::GetPlayerPed,
		mem(&Game::Ped::copValue)
	);
}

void ModMenuModule::CopValueCheat::OnEnable()
{
	Core::WatchManager* watchManager = Core::WatchManager::GetInstance();
	m_watchedCopValue = watchManager->Watch<GameTickEvent>(m_copValueResolver, this, &ModMenuModule::CopValueCheat::OnValueUpdate);

	UpdateCopValueChangeListener();
	AddEventListener<GameEndEvent>(&ModMenuModule::CopValueCheat::OnGameEnd);
}

void ModMenuModule::CopValueCheat::OnDisable()
{
	SaveToPersistence();

	RemoveEventListener<GameEndEvent>();
	if (HasEventListener<CopValueChangeEvent>()) {
		RemoveEventListener<CopValueChangeEvent>();
	}

	Core::WatchManager* watchManager = Core::WatchManager::GetInstance();

	watchManager->Unwatch(m_watchedCopValue);
	m_watchedCopValue = nullptr;
}

void ModMenuModule::CopValueCheat::UpdateCopValueChangeListener()
{
	SetEventListener<CopValueChangeEvent>(&ModMenuModule::CopValueCheat::OnCopValueChange, m_lockedCopValue.has_value());
}

void ModMenuModule::CopValueCheat::OnValueUpdate(const std::optional<short>& oldValue, const std::optional<short>& newValue)
{
	if (m_lockedCopValue.has_value() && newValue.has_value()) {
		m_watchedCopValue->SetValueNow(m_lockedCopValue.value(), false);
	}
}

void ModMenuModule::CopValueCheat::OnCopValueChange(CopValueChangeEvent& event)
{
	Game::Ped* playerPed = Game::Memory::GetPlayerPed();
	if (event.GetPed() != playerPed) return;

	const auto& savedValue = m_watchedCopValue->GetSavedValue();
	if (!savedValue.has_value()) {
		return;
	}
	event.SetModifiedNewValue(savedValue.value());
}

void ModMenuModule::CopValueCheat::OnGameEnd(GameEndEvent& event)
{
	if (!m_lockedCopValue.has_value()) {
		m_watchedCopValue->CancelSetValue();
	}
}

void ModMenuModule::CopValueCheat::SaveToPersistence() const
{
	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();
	persistence->Save("Cheat_CopValue_LockedCopValue", m_lockedCopValue);
}

void ModMenuModule::CopValueCheat::LoadFromPersistence()
{
	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();
	m_lockedCopValue = persistence->Load<std::optional<short>>("Cheat_CopValue_LockedCopValue", std::nullopt);
}

REGISTER_CHEAT(CopValueCheat)
