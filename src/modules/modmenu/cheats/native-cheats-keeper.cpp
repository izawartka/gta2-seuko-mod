#include "native-cheats-keeper.h"
#include "../cheat-registry.h"
#include "../../../events/game-tick.h"
#include "native-cheats-data.cpp"

ModMenuModule::NativeCheatsKeeperCheat::NativeCheatsKeeperCheat() : ModMenuModule::CheatBase("Cheat_NativeCheatsKeeper_IsEnabled")
{
}

ModMenuModule::NativeCheatsKeeperCheat::~NativeCheatsKeeperCheat()
{
}

const std::vector<ModMenuModule::NativeCheatDef>& ModMenuModule::NativeCheatsKeeperCheat::GetAllNativeCheatsByCategory(NativeCheatCategory category)
{
	return nativeCheatsByCategoryMap.at(category);
}

const std::vector<ModMenuModule::NativeCheatDef>& ModMenuModule::NativeCheatsKeeperCheat::GetAllNativeCheats()
{
	return nativeCheats;
}

const std::vector<ModMenuModule::NativeCheatCategoryDef>& ModMenuModule::NativeCheatsKeeperCheat::GetAllNativeCheatCategories()
{
	return nativeCheatCategories;
}

void ModMenuModule::NativeCheatsKeeperCheat::SetCheat(const NativeCheatDef& cheat, NativeCheatState state)
{
	if (!IsEnabled()) {
		spdlog::warn("NativeCheatsKeeperCheat::SetCheat: Cheat keeper is not enabled, cannot set cheat state");
		return;
	}
	if (cheat.address >= sizeof(Game::Cheats)) {
		spdlog::error("NativeCheatsKeeperCheat::SetCheat: Invalid cheat address: {}", cheat.address);
		return;
	}

	spdlog::debug("Setting cheat {} state to {}", static_cast<size_t>(cheat.address), static_cast<int>(state));
	SetCheatStateInternal(cheat.address, state);
}

ModMenuModule::NativeCheatState ModMenuModule::NativeCheatsKeeperCheat::GetCheatState(const NativeCheatDef& cheat) const
{
	if (!IsEnabled()) return NativeCheatState::Unchanged;
	if (cheat.address >= sizeof(Game::Cheats)) {
		spdlog::error("NativeCheatsKeeperCheat::GetCheatState: Invalid cheat address: {}", cheat.address);
		return NativeCheatState::Unchanged;
	}

	return m_cheatEntries[cheat.address].state;
}

bool ModMenuModule::NativeCheatsKeeperCheat::IsCheatEnabled(const NativeCheatDef& cheat) const
{
	if (cheat.address >= sizeof(Game::Cheats)) {
		spdlog::error("NativeCheatsKeeperCheat::IsCheatEnabled: Invalid cheat address: {}", cheat.address);
		return false;
	}

	Game::Cheats* cheats = Game::Memory::GetCheats();
	bool* cheatBytes = reinterpret_cast<bool*>(cheats);
	return cheatBytes[cheat.address];
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

void ModMenuModule::NativeCheatsKeeperCheat::OnCheatValueChange(size_t cheatAddress, std::optional<bool> oldValue, std::optional<bool> newValue)
{
	NativeCheatsKeeperEntry& entry = m_cheatEntries[cheatAddress];
	if (entry.ignoreValueChange) {
		entry.ignoreValueChange = false;
		return;
	}

	spdlog::debug("NativeCheatsKeeperCheat::OnCheatValueChange: Cheat at address {} changed from {} to {}", cheatAddress,
		oldValue.has_value() ? (oldValue.value() ? "true" : "false") : "null",
		newValue.has_value() ? (newValue.value() ? "true" : "false") : "null"
	);

	if (!newValue.has_value()) return;
	entry.originalValue = newValue.value();
	entry.watched->SetValueNoNotify(entry.state == NativeCheatState::ForceEnabled);
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

void ModMenuModule::NativeCheatsKeeperCheat::SetCheatStateInternal(size_t cheatAddress, NativeCheatState state)
{
	Core::WatchManager* watchManager = Core::WatchManager::GetInstance();
	bool& cheatValue = (reinterpret_cast<bool*>(Game::Memory::GetCheats()))[cheatAddress];
	NativeCheatsKeeperEntry& entry = m_cheatEntries[cheatAddress];

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
			[cheatAddress]() { return &(reinterpret_cast<bool*>(Game::Memory::GetCheats()))[cheatAddress]; },
			std::bind(&ModMenuModule::NativeCheatsKeeperCheat::OnCheatValueChange, this, cheatAddress, std::placeholders::_1, std::placeholders::_2)
		);
		entry.watched->Update();
	}

	entry.watched->SetValueNoNotify(state == NativeCheatState::ForceEnabled);
}

REGISTER_CHEAT(NativeCheatsKeeperCheat)
