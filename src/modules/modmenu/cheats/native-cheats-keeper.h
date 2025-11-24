#pragma once
#include "../common.h"
#include "../cheat-base.h"
#include "../../../events/after-debug-flags.h"

namespace ModMenuModule {
	enum class NativeCheatCategory {
		Standard,
		SkipThings,
		DebugInfo,
		TilesRelated,
		RequiringRestart,
		ReplayRelated,
		UnstableOrUnused,
		Logging,
		OtherDebug
	};

	struct NativeCheatCategoryDef {
		NativeCheatCategory category;
		std::wstring name;
	};

	struct NativeCheatDef {
		NativeCheatCategory category;
		std::wstring name;
		size_t address;
	};

	enum class NativeCheatState {
		ForceDisabled = -1,
		Unchanged = 0,
		ForceEnabled = 1,
	};

	class NativeCheatsKeeperCheat : public CheatBase, public Core::EventListenerSupport {
	public:
		NativeCheatsKeeperCheat();
		virtual ~NativeCheatsKeeperCheat();

		static const std::vector<NativeCheatDef>& GetAllNativeCheats();
		static const std::vector<NativeCheatDef>& GetAllNativeCheatsByCategory(NativeCheatCategory category);
		static const std::vector<NativeCheatCategoryDef>& GetAllNativeCheatCategories();

		void SetCheat(const NativeCheatDef& cheat, NativeCheatState state);
		NativeCheatState GetCheatState(const NativeCheatDef& cheat) const;
		bool IsCheatEnabled(const NativeCheatDef& cheat) const;
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
		void OnCheatValueChange(size_t cheatAddress, const std::optional<bool>& oldValue, const std::optional<bool>& newValue);

		virtual void OnFirstEnable() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

		void SaveToPersistence() const;
		void LoadFromPersistence();
		void SetCheatStateInternal(size_t cheatAddress, NativeCheatState state);

		NativeCheatsKeeperEntry m_cheatEntries[sizeof(Game::Cheats)] = {};
	};
}
