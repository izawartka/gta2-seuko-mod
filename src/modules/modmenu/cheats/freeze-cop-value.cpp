#include "freeze-cop-value.h"
#include "../../../events/game-tick.h"
#include "../cheat-registry.h"

ModMenuModule::FreezeCopValue::FreezeCopValue()
	: ModMenuModule::CheatBase("Cheat_FreezeCopValue_IsEnabled") {
}

ModMenuModule::FreezeCopValue::~FreezeCopValue()
{

}

void ModMenuModule::FreezeCopValue::SetCopValue(short copValue)
{
	m_copValue = copValue;
	if(m_watchedCopValue) {
		m_watchedCopValue->RequestUpdate();
	}
}

void ModMenuModule::FreezeCopValue::OnFirstEnable()
{
	m_copValueResolver = Core::MakeResolver(
		Game::Memory::GetPlayerPed,
		mem(&Game::Ped::copValue)
	);
}

void ModMenuModule::FreezeCopValue::OnEnable()
{
	Core::WatchManager* watchManager = Core::WatchManager::GetInstance();
	m_watchedCopValue = watchManager->Watch<GameTickEvent, short>(m_copValueResolver, this, &ModMenuModule::FreezeCopValue::OnValueUpdate);
	m_justEnabled = true;
}

void ModMenuModule::FreezeCopValue::OnDisable()
{
	Core::WatchManager* watchManager = Core::WatchManager::GetInstance();
	watchManager->Unwatch<short>(m_watchedCopValue);
	m_watchedCopValue = nullptr;
}

void ModMenuModule::FreezeCopValue::OnValueUpdate(std::optional<short> oldValue, std::optional<short> newValue)
{
	if (!newValue.has_value() || newValue.value() == m_copValue) return;

	if (m_justEnabled) {
		m_justEnabled = false;
		m_copValue = newValue.value();
		return;
	}

	m_watchedCopValue->SetValue(m_copValue);
}

REGISTER_CHEAT(FreezeCopValue)
