#include "set-cop-value.h"
#include "freeze-cop-value.h"
#include "../cheat-registry.h"

ModMenuModule::SetCopValueCheat* ModMenuModule::SetCopValueCheat::m_instance = nullptr;

ModMenuModule::SetCopValueCheat::SetCopValueCheat() : ModMenuModule::CheatBase("") {
	assert(m_instance == nullptr && "SetCopValueCheat instance already exists");
	m_instance = this;
}

ModMenuModule::SetCopValueCheat::~SetCopValueCheat()
{
	m_instance = nullptr;
}

ModMenuModule::SetCopValueCheat* ModMenuModule::SetCopValueCheat::GetInstance()
{
	assert(m_instance && "SetCopValueCheat not initialized!");
	return m_instance;
}

void ModMenuModule::SetCopValueCheat::SetCopValueAndDisable(short copValue)
{
	if (!IsEnabled()) {
		spdlog::error("SetCopValueCheat::SetCopValueAndDisable: Cheat is not enabled, cannot set cop value");
		return;
	}

	FreezeCopValueCheat* freezeCopValueCheat = FreezeCopValueCheat::GetInstance();
	if (freezeCopValueCheat->IsEnabled()) {
		spdlog::debug("SetCopValueCheat::SetCopValueAndDisable: Forwarding to FreezeCopValueCheat");
		freezeCopValueCheat->SetFrozenCopValue(copValue);
		SetEnabled(false);
		return;
	}

	m_pendingValue = copValue;
}

void ModMenuModule::SetCopValueCheat::OnFirstEnable()
{
	m_copValueResolver = Core::MakeResolver(
		Game::Memory::GetPlayerPed,
		mem(&Game::Ped::copValue)
	);
}

void ModMenuModule::SetCopValueCheat::OnEnable()
{
	AddEventListener<GameTickEvent>(&ModMenuModule::SetCopValueCheat::OnGameTick, true);
	AddEventListener<GameEndEvent>(&ModMenuModule::SetCopValueCheat::OnGameEnd, true);
}

void ModMenuModule::SetCopValueCheat::OnDisable()
{
	if(HasEventListener<GameTickEvent>()) RemoveEventListener<GameTickEvent>();
	if(HasEventListener<GameEndEvent>()) RemoveEventListener<GameEndEvent>();
	m_pendingValue = std::nullopt;
}

void ModMenuModule::SetCopValueCheat::OnGameTick(GameTickEvent& event)
{
	if (!m_pendingValue.has_value()) {
		spdlog::warn("SetCopValueCheat::OnGameTick: Executed without a value set");
		SetEnabled(false);
		return;
	}

	FreezeCopValueCheat* freezeCopValueCheat = FreezeCopValueCheat::GetInstance();
	if (freezeCopValueCheat->IsEnabled()) {
		spdlog::debug("SetCopValueCheat::OnGameTick: Forwarding to FreezeCopValueCheat");
		freezeCopValueCheat->SetFrozenCopValue(m_pendingValue.value());
		SetEnabled(false);
		return;
	}

	short* copValuePtr = m_copValueResolver();
	if (!copValuePtr) {
		spdlog::error("SetCopValueCheat::OnGameTick: Could not resolve copValue");
	}
	else {
		*copValuePtr = m_pendingValue.value();
		spdlog::debug("SetCopValueCheat: Cop value set to {}", m_pendingValue.value());
	}

	SetEnabled(false);
}

void ModMenuModule::SetCopValueCheat::OnGameEnd(GameEndEvent& event)
{
	spdlog::debug("SetCopValueCheat: Detected game end, skipping value set");
	m_pendingValue = std::nullopt;

	SetEnabled(false);
}

REGISTER_CHEAT(SetCopValueCheat)
