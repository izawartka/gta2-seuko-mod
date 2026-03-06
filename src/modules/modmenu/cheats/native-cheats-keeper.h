#pragma once
#include "../common.h"
#include "../cheat-base.h"
#include "../../../events/after-debug-flags.h"

namespace ModMenuModule {
	enum class NativeCheatState {
		ForceDisabled = -1,
		Unchanged = 0,
		ForceEnabled = 1,
	};

	class NativeCheatsKeeperCheat : public CheatBase, public Core::EventListenerSupport {
	public:
		NativeCheatsKeeperCheat();
		virtual ~NativeCheatsKeeperCheat() override;
		static NativeCheatsKeeperCheat* GetInstance();

		static bool IsCheatEnabled(size_t index);
		bool SetCheat(size_t index, NativeCheatState state);
		NativeCheatState GetCheatState(size_t index) const;
		void ResetAll();

	private:
		struct NativeCheatsKeeperEntry {
			bool originalValue = false;
			bool ignoreValueChange = false;
			NativeCheatState state = NativeCheatState::Unchanged;
			Core::Watched<bool>* watched = nullptr;
		};

		struct NativeCheatsPersistenceData {
			NativeCheatState cheatStates[sizeof(Game::Cheats)] = { NativeCheatState::Unchanged };
		};

		void OnAfterDebugFlags(AfterDebugFlagsEvent& event);
		void OnCheatValueChange(size_t index, const std::optional<bool>& oldValue, const std::optional<bool>& newValue);

		virtual void OnFirstEnable() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

		void SaveToPersistence() const;
		void LoadFromPersistence();
		void SetCheatStateInternal(size_t index, NativeCheatState state);

		static NativeCheatsKeeperCheat* m_instance;
		NativeCheatsKeeperEntry m_cheatEntries[sizeof(Game::Cheats)] = {};
	};
}
