#include "freeze-cop-value.h"
#include "../../../events/game-tick.h"
#include "../cheat-registry.h"

ModMenuModule::FreezeCopValueCheat::FreezeCopValueCheat()
	: ModMenuModule::CheatBase("Cheat_FreezeCopValue_IsEnabled") {
}

ModMenuModule::FreezeCopValueCheat::~FreezeCopValueCheat()
{

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
	m_watchedCopValue = watchManager->Watch<GameTickEvent, short>(m_copValueResolver, this, &ModMenuModule::FreezeCopValueCheat::OnValueUpdate);
	m_justEnabled = true;
}

void ModMenuModule::FreezeCopValueCheat::OnDisable()
{
	Core::WatchManager* watchManager = Core::WatchManager::GetInstance();
	watchManager->Unwatch<short>(m_watchedCopValue);
	m_watchedCopValue = nullptr;
}

void ModMenuModule::FreezeCopValueCheat::OnValueUpdate(std::optional<short> oldValue, std::optional<short> newValue)
{
	if (!newValue.has_value() || newValue.value() == m_copValue) return;

	if (m_justEnabled) {
		m_justEnabled = false;
		m_copValue = newValue.value();
		return;
	}

	m_watchedCopValue->SetValue(m_copValue);
}

REGISTER_CHEAT(FreezeCopValueCheat)
