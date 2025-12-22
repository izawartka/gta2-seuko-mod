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

void ModMenuModule::CopValueCheat::SetCopValueFrozen(bool frozen)
{
	if (!IsEnabled()) {
		spdlog::error("CopValueCheat::SetCopValueFrozen: Cheat is not enabled, cannot set frozen state");
		return;
	}

	m_isCopValueFrozen = frozen;
	UpdateCopValueChangeListener();
}

bool ModMenuModule::CopValueCheat::IsCopValueFrozen() const
{
	return IsEnabled() && m_isCopValueFrozen;
}

void ModMenuModule::CopValueCheat::SetCopValue(short copValue)
{
	if (!IsEnabled()) {
		spdlog::error("CopValueCheat::SetCopValue: Cheat is not enabled, cannot set cop value");
		return;
	}
	m_watchedCopValue->SetValue(copValue);
}

const std::optional<short>& ModMenuModule::CopValueCheat::GetCopValue() const
{
	if (!IsEnabled()) {
		spdlog::error("CopValueCheat::GetCopValue: Cheat is not enabled, cannot get cop value");
		static const std::optional<short> emptyValue = std::nullopt;
		return emptyValue;
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
}

void ModMenuModule::CopValueCheat::OnDisable()
{
	SaveToPersistence();

	if (HasEventListener<CopValueChangeEvent>()) {
		RemoveEventListener<CopValueChangeEvent>();
	}

	Core::WatchManager* watchManager = Core::WatchManager::GetInstance();
	watchManager->Unwatch(m_watchedCopValue);
	m_watchedCopValue = nullptr;
}

void ModMenuModule::CopValueCheat::UpdateCopValueChangeListener()
{
	SetEventListener<CopValueChangeEvent>(&ModMenuModule::CopValueCheat::OnCopValueChange, m_isCopValueFrozen);
}

void ModMenuModule::CopValueCheat::OnValueUpdate(const std::optional<short>& oldValue, const std::optional<short>& newValue)
{
	if (m_isCopValueFrozen && oldValue.has_value()) {
		m_watchedCopValue->SetValueNow(oldValue.value(), false);
	}
}

void ModMenuModule::CopValueCheat::OnCopValueChange(CopValueChangeEvent& event)
{
	const auto& savedValue = m_watchedCopValue->GetSavedValue();
	if (!savedValue.has_value()) {
		return;
	}
	event.SetModifiedNewValue(savedValue.value());
}

void ModMenuModule::CopValueCheat::SaveToPersistence() const
{
	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();
	persistence->Save<bool>("Cheat_CopValue_IsFrozen", m_isCopValueFrozen);
}

void ModMenuModule::CopValueCheat::LoadFromPersistence()
{
	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();
	m_isCopValueFrozen = persistence->Load<bool>("Cheat_CopValue_IsFrozen", false);
}

REGISTER_CHEAT(CopValueCheat)
