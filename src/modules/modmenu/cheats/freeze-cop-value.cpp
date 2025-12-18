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

void ModMenuModule::FreezeCopValueCheat::SetCopValue(short copValue)
{
	m_copValue = copValue;
	if(m_watchedCopValue) {
		m_watchedCopValue->RequestUpdate();
	}
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
	m_justEnabled = true;
	m_watchedCopValue->Update();

	AddEventListener<CopValueChangeEvent>(&ModMenuModule::FreezeCopValueCheat::OnCopValueChange);
}

void ModMenuModule::FreezeCopValueCheat::OnDisable()
{
	RemoveEventListener<CopValueChangeEvent>();

	Core::WatchManager* watchManager = Core::WatchManager::GetInstance();
	watchManager->Unwatch(m_watchedCopValue);
	m_watchedCopValue = nullptr;
}

void ModMenuModule::FreezeCopValueCheat::OnValueUpdate(const std::optional<short>& oldValue, const std::optional<short>& newValue)
{
	if (m_justEnabled) {
		m_copValue = newValue.value_or(0);
		m_justEnabled = false;
		return;
	}

	m_watchedCopValue->SetValueNow(m_copValue);
}

void ModMenuModule::FreezeCopValueCheat::OnCopValueChange(CopValueChangeEvent& event)
{
	event.SetModifiedNewValue(m_copValue);
}

REGISTER_CHEAT(FreezeCopValueCheat)
