#pragma once
#include "../common.h"
#include "../cheat-base.h"

namespace ModMenuModule {
	struct SavedCarsCheatEntry {
		Game::CAR_MODEL4 model;
		short remap;
		Game::PALETTE_BASE palette;
		enum Game::CAR_DELTAS_BITSTATE deltasBitstate;
		short carDamage;
		short physicsBitmask;
		bool hasTankCannon;
		bool hasWaterCannon;
		bool hasJeepTurret;
		short carWeaponsAmmo[13];
	};

	class SavedCarsCheat : public CheatBase {
	public:
		SavedCarsCheat();
		virtual ~SavedCarsCheat() override;
		static SavedCarsCheat* GetInstance();

		bool SaveCar(const std::wstring& name, Game::Car* car);
		std::optional<SavedCarsCheatEntry> GetCar(const std::wstring& name) const;
		bool SpawnCar(const std::wstring& name) const;
		bool DeleteCar(const std::wstring& name);
		const std::vector<std::wstring>& GetSavedCarsList();

	private:
		virtual void OnFirstEnable() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;
		virtual bool AutoEnableOnAttach() const override { return true; }

		static SavedCarsCheatEntry CreateEntry(Game::Car* car);
		static bool SpawnEntry(const SavedCarsCheatEntry& entry);

		void SaveToPersistence() const;
		void LoadFromPersistence();
		void RefreshSortedCache();

		static SavedCarsCheat* m_instance;
		std::unordered_map<std::wstring, SavedCarsCheatEntry> m_entries = {};
		std::vector<std::wstring> m_sortedCache = {};
		bool m_cacheDirty = true;
	};
}
