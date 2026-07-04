#pragma once
#include "../common.h"
#include "../cheat-base.h"
#include "player-pos.h"
#include "../events/player-pos-rot-update.h"

namespace ModMenuModule {
	struct PositionStoreValue {
		Game::SCR_Vector3 position = { 0, 0, 0 };
		short rotation = 0;

		bool operator==(const PositionStoreValue& other) const {
			return position == other.position && rotation == other.rotation;
		}

		bool operator!=(const PositionStoreValue& other) const {
			return !(*this == other);
		}
	};

	struct PositionStoreEntry {
		PositionStoreValue value = {};
		bool updateFromPlayerPed = true;
		bool autoZ = true;

		bool operator==(const PositionStoreEntry& other) const {
			return value == other.value && 
				updateFromPlayerPed == other.updateFromPlayerPed && 
				autoZ == other.autoZ;
		}

		bool operator!=(const PositionStoreEntry& other) const {
			return !(*this == other);
		}
	};

	class PositionStoreCheat : public CheatBase, public Core::EventListenerSupport {
	public:
		using PositionId = size_t;

		PositionStoreCheat();
		virtual ~PositionStoreCheat() override;
		static PositionStoreCheat* GetInstance();

		static auto GetDependencies() {
			return std::array<std::type_index, 1> { typeid(PlayerPosCheat) };
		}

		PositionId Create();
		PositionId Create(const PositionStoreEntry& entry);
		bool Update(PositionId id, const PositionStoreEntry& entry);
		bool Remove(PositionId id);
		const PositionStoreEntry* Get(PositionId id) const;

		static void ApplyNow(PositionStoreEntry& entry);

		static void SaveToPersistence(const std::string& key, const PositionStoreEntry& entry);
		static std::optional<PositionStoreEntry> LoadFromPersistence(const std::string& key);
		static bool ConvertPersistence(std::unique_ptr<uint8_t[]>& dataPtr, size_t& dataSize, uint8_t version);

	private:
		virtual void OnFirstEnable() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

		static bool ApplyFromPlayerNow(PositionStoreValue& value);
		static bool ApplyAutoZ(PositionStoreValue& value);

		void UpdatePlayerUpdateSet(PositionStoreEntry* entry, bool add);
		void OnPlayerPosRotUpdate(ModMenuModule::PlayerPosRotUpdateEvent& event);

		static PositionStoreCheat* m_instance;
		std::unordered_map<PositionId, PositionStoreEntry> m_entries = {};
		std::set<PositionStoreEntry*> m_playerUpdateEntries = {};
		PositionId m_nextPositionId = 1;
	};
}
