#include "native-cheats-keeper.h"
#include "../cheat-registry.h"
#include "../../../events/game-tick.h"

ModMenuModule::NativeCheatsKeeperCheat* ModMenuModule::NativeCheatsKeeperCheat::m_instance = nullptr;

ModMenuModule::NativeCheatsKeeperCheat::NativeCheatsKeeperCheat() : ModMenuModule::CheatBase("Cheat_NativeCheatsKeeper_IsEnabled")
{
	assert(m_instance == nullptr && "NativeCheatsKeeperCheat instance already exists");
	m_instance = this;
}

ModMenuModule::NativeCheatsKeeperCheat::~NativeCheatsKeeperCheat()
{
	m_instance = nullptr;
}

ModMenuModule::NativeCheatsKeeperCheat* ModMenuModule::NativeCheatsKeeperCheat::GetInstance()
{
	assert(m_instance && "NativeCheatsKeeperCheat not initialized!");
	return m_instance;
}

bool ModMenuModule::NativeCheatsKeeperCheat::IsCheatEnabled(size_t index)
{
	if (index >= sizeof(Game::Cheats)) {
		spdlog::error("NativeCheatsKeeperCheat::IsCheatEnabled: Invalid cheat index: {}", index);
		return false;
	}

	Game::Cheats* cheats = Game::Memory::GetCheats();
	bool* cheatBytes = reinterpret_cast<bool*>(cheats);
	return cheatBytes[index];
}

bool ModMenuModule::NativeCheatsKeeperCheat::SetCheat(size_t index, NativeCheatState state)
{
	if (!IsEnabled()) {
		spdlog::warn("NativeCheatsKeeperCheat::SetCheat: Cheat keeper is not enabled, cannot set cheat state");
		return false;
	}
	if (index >= sizeof(Game::Cheats)) {
		spdlog::error("NativeCheatsKeeperCheat::SetCheat: Invalid cheat index: {}", index);
		return false;
	}
	spdlog::debug("Setting cheat {} state to {}", static_cast<size_t>(index), static_cast<int>(state));
	SetCheatStateInternal(index, state);
	return true;
}

ModMenuModule::NativeCheatState ModMenuModule::NativeCheatsKeeperCheat::GetCheatState(size_t index) const
{
	if (!IsEnabled()) return NativeCheatState::Unchanged;
	if (index >= sizeof(Game::Cheats)) {
		spdlog::error("NativeCheatsKeeperCheat::GetCheatState: Invalid cheat index: {}", index);
		return NativeCheatState::Unchanged;
	}

	return m_cheatEntries[index].state;
}

void ModMenuModule::NativeCheatsKeeperCheat::ResetAll()
{
	spdlog::debug("NativeCheatsKeeperCheat::ResetAll: Resetting all cheats to Unchanged");
	for (size_t i = 0; i < sizeof(Game::Cheats); i++) {
		if (m_cheatEntries[i].state == NativeCheatState::Unchanged) continue;
		SetCheatStateInternal(i, NativeCheatState::Unchanged);
	}
}

void ModMenuModule::NativeCheatsKeeperCheat::OnAfterDebugFlags(AfterDebugFlagsEvent& event)
{
	for (size_t i = 0; i < sizeof(Game::Cheats); i++) {
		NativeCheatsKeeperEntry& entry = m_cheatEntries[i];
		if (entry.watched) {
			entry.watched->Update();
		}
	}
}

void ModMenuModule::NativeCheatsKeeperCheat::OnCheatValueChange(size_t index, const std::optional<bool>& oldValue, const std::optional<bool>& newValue)
{
	NativeCheatsKeeperEntry& entry = m_cheatEntries[index];
	if (entry.ignoreValueChange) {
		entry.ignoreValueChange = false;
		return;
	}

	spdlog::debug("NativeCheatsKeeperCheat::OnCheatValueChange: Cheat with index {} changed from {} to {}", index,
		oldValue.has_value() ? (oldValue.value() ? "true" : "false") : "null",
		newValue.has_value() ? (newValue.value() ? "true" : "false") : "null"
	);

	if (!newValue.has_value()) return;
	entry.originalValue = newValue.value();
	entry.watched->SetValueNow(entry.state == NativeCheatState::ForceEnabled);
}

void ModMenuModule::NativeCheatsKeeperCheat::OnFirstEnable()
{
	LoadFromPersistence();
}

void ModMenuModule::NativeCheatsKeeperCheat::OnEnable()
{
	AddEventListener<AfterDebugFlagsEvent>(&NativeCheatsKeeperCheat::OnAfterDebugFlags);
}

void ModMenuModule::NativeCheatsKeeperCheat::OnDisable()
{
	RemoveEventListener<AfterDebugFlagsEvent>();
	SaveToPersistence();
	ResetAll();
}

void ModMenuModule::NativeCheatsKeeperCheat::SaveToPersistence() const
{
	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();
	NativeCheatsPersistenceData persistenceData;
	for (size_t i = 0; i < sizeof(Game::Cheats); i++) {
		persistenceData.cheatStates[i] = m_cheatEntries[i].state;
	}
	persistence->Save<NativeCheatsPersistenceData>("Cheat_NativeCheatsKeeper_State", persistenceData);
}

void ModMenuModule::NativeCheatsKeeperCheat::LoadFromPersistence()
{
	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();
	auto persistenceData = persistence->LoadOptional<NativeCheatsPersistenceData>("Cheat_NativeCheatsKeeper_State");
	if (!persistenceData.has_value()) return;
	for (size_t i = 0; i < sizeof(Game::Cheats); i++) {
		SetCheatStateInternal(i, persistenceData->cheatStates[i]);
	}
}

void ModMenuModule::NativeCheatsKeeperCheat::SetCheatStateInternal(size_t index, NativeCheatState state)
{
	Core::WatchManager* watchManager = Core::WatchManager::GetInstance();
	bool& cheatValue = (reinterpret_cast<bool*>(Game::Memory::GetCheats()))[index];
	NativeCheatsKeeperEntry& entry = m_cheatEntries[index];

	entry.state = state;
	if (state == NativeCheatState::Unchanged) {
		if (entry.watched) {
			watchManager->Unwatch(entry.watched);
			entry.watched = nullptr;
			cheatValue = entry.originalValue;
		}

		return;
	}

	if (entry.watched == nullptr) {
		entry.originalValue = cheatValue;
		entry.ignoreValueChange = true;
		entry.watched = watchManager->Watch<PreGameTickEvent, bool, bool*>(
			[index]() { return &(reinterpret_cast<bool*>(Game::Memory::GetCheats()))[index]; },
			std::bind(&ModMenuModule::NativeCheatsKeeperCheat::OnCheatValueChange, this, index, std::placeholders::_1, std::placeholders::_2)
		);
		entry.watched->Update();
	}

	entry.watched->SetValueNow(state == NativeCheatState::ForceEnabled);
}

REGISTER_CHEAT(NativeCheatsKeeperCheat)
