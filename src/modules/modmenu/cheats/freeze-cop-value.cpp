#include "freeze-cop-value.h"
#include "../../../events/game-tick.h"
#include "../cheat-registry.h"

ModMenuModule::FreezeCopValueCheat* ModMenuModule::FreezeCopValueCheat::m_instance = nullptr;

ModMenuModule::FreezeCopValueCheat::FreezeCopValueCheat()
	: ModMenuModule::CheatBase("Cheat_FreezeCopValue_IsEnabled") {
	assert(m_instance == nullptr && "FreezeCopValueCheat instance already exists");
	m_instance = this;
}

ModMenuModule::FreezeCopValueCheat::~FreezeCopValueCheat()
{
	m_instance = nullptr;
}

ModMenuModule::FreezeCopValueCheat* ModMenuModule::FreezeCopValueCheat::GetInstance()
{
	assert(m_instance && "FreezeCopValueCheat not initialized!");
	return m_instance;
}

void ModMenuModule::FreezeCopValueCheat::SetFrozenCopValue(short copValue)
{
	if (!IsEnabled()) {
		spdlog::error("FreezeCopValueCheat::SetCopValueCheat: Cheat is not enabled, cannot set cop value");
		return;
	}

	m_copValue = copValue;
	m_watchedCopValue->RequestUpdate();
}

void ModMenuModule::FreezeCopValueCheat::OnFirstEnable()
{
	m_copValueResolver = Core::MakeResolver(
		Game::Memory::GetPlayerPed,
		mem(&Game::Ped::copValue)
	);
}

void ModMenuModule::FreezeCopValueCheat::OnEnable()
{
	Core::WatchManager* watchManager = Core::WatchManager::GetInstance();
	m_watchedCopValue = watchManager->Watch<GameTickEvent>(m_copValueResolver, this, &ModMenuModule::FreezeCopValueCheat::OnValueUpdate);

	AddEventListener<CopValueChangeEvent>(&ModMenuModule::FreezeCopValueCheat::OnCopValueChange);
}

void ModMenuModule::FreezeCopValueCheat::OnDisable()
{
	RemoveEventListener<CopValueChangeEvent>();

	Core::WatchManager* watchManager = Core::WatchManager::GetInstance();
	watchManager->Unwatch(m_watchedCopValue);
	m_watchedCopValue = nullptr;
	m_copValue = std::nullopt;
}

void ModMenuModule::FreezeCopValueCheat::OnValueUpdate(const std::optional<short>& oldValue, const std::optional<short>& newValue)
{
	if (m_copValue.has_value()) {
		m_watchedCopValue->SetValueNow(m_copValue.value());
	}
	else {
		m_copValue = newValue.value_or(0);
		return;
	}
}

void ModMenuModule::FreezeCopValueCheat::OnCopValueChange(CopValueChangeEvent& event)
{
	event.SetModifiedNewValue(m_copValue.value());
}

REGISTER_CHEAT(FreezeCopValueCheat)
