#pragma once
#include "../common.h"
#include "../cheat-base.h"

namespace ModMenuModule {
	struct SavedCarsCheatEntry {
		Game::CAR_MODEL4 model;
		short remap;
		Game::PALETTE_BASE palette;
		enum Game::CAR_LIGHTS_AND_DOORS_BITSTATE carLights;
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

		bool SaveCar(std::wstring name, Game::Car* car);
		std::optional<SavedCarsCheatEntry> GetCar(std::wstring name);
		bool SpawnCar(std::wstring name);
		bool DeleteCar(std::wstring name);
		std::vector<std::wstring> GetSavedCarsList();

	private:
		virtual void OnFirstEnable() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;
		virtual bool AutoEnableOnAttach() const override { return true; }

		static SavedCarsCheatEntry CreateEntry(Game::Car* car);
		static bool SpawnEntry(const SavedCarsCheatEntry& entry);

		void SaveToPersistence() const;
		void LoadFromPersistence();

		static SavedCarsCheat* m_instance;
		std::unordered_map<std::wstring, SavedCarsCheatEntry> m_entries = {};
	};
}
