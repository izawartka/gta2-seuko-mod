#pragma once
#include "common.h"
#include "types.h"
#include "watched-base.h"
#include "event-base.h"
#include "event-manager.h"

namespace Core
{
	class WatchManager {
	public:
		static WatchManager* GetInstance();

		template<typename EventT, typename ValueT, typename ResRetT>
		Watched<ValueT, ResRetT>* Watch(Resolver<ResRetT> resolver, WatchedListener<ValueT> listener)
		{
			static_assert(std::is_copy_constructible<ValueT>::value, "ValueT must be copy-constructible");
			static_assert(std::is_base_of_v<EventBase, EventT> || std::is_same<EventT, void>::value, "EventT must derive from Core::Event or be void");

			auto entry = new Watched<ValueT, ResRetT>(m_nextEntryId++, std::move(resolver), std::move(listener));
			
			if (m_updating) {
				PendingChange change(ChangeType::Add, entry->m_id, std::type_index(typeid(EventT)), entry);
				m_pendingChanges.push(change);
			}
			else {
				RegisterEntry<EventT>(entry);
			}
			
			return entry;
		}

		template<typename EventT, typename ValueT, typename ResRetT, typename U>
		Watched<ValueT, ResRetT>* Watch(Resolver<ResRetT> resolver, U* instance, WatchedMethodListener<ValueT, U> method)
		{
			static_assert(std::is_copy_constructible<ValueT>::value, "ValueT must be copy-constructible");
			static_assert(std::is_base_of_v<EventBase, EventT> || std::is_same<EventT, void>::value, "EventT must derive from Core::Event or be void");

			auto listener = [instance, method](const std::optional<ValueT>& oldVal, const std::optional<ValueT>& newVal) {
				(instance->*method)(oldVal, newVal);
			};
			return Watch<EventT, ValueT, ResRetT>(std::move(resolver), std::move(listener));
		}

		void Unwatch(WatchedId id) {
			spdlog::debug("Removing watched with id: {}", id);

			auto it = std::find_if(m_idToEventType.begin(), m_idToEventType.end(),
				[id](const auto& pair) { return pair.first == id; });

			if (it == m_idToEventType.end()) {
				spdlog::warn("No watched found with id: {}", id);
				return;
			}

			auto eventTypeIdx = it->second;
			
			if (m_updating) {
				PendingChange change(ChangeType::Remove, id, eventTypeIdx, nullptr);
				m_pendingChanges.push(change);
			}
			else {
				RemoveEntry(id, eventTypeIdx);
			}
		}

		void Unwatch(WatchedBase* watched) {
			if (watched == nullptr) return;
			Unwatch(watched->GetId());
		}

	private:
		friend class Core;
		WatchManager();
		~WatchManager();
		WatchManager(const WatchManager&) = delete;
		WatchManager& operator=(const WatchManager&) = delete;

		enum class ChangeType { Add, Remove };

		struct PendingChange {
			ChangeType type;
			WatchedId id;
			std::type_index eventType;
			WatchedBase* entry; // Only used for Add
			
			PendingChange(ChangeType t, WatchedId i, std::type_index et, WatchedBase* e)
				: type(t), id(i), eventType(et), entry(e) {}
		};

		template<typename EventT>
		void RegisterEntry(WatchedBase* entry) {
			WatchedId id = entry->m_id;
			spdlog::debug("Adding watched: {} with id: {} (Event: {})", typeid(*entry).name(), id, typeid(EventT).name());

			EnsureEventType<EventT>();

			auto eventTypeIdx = std::type_index(typeid(EventT));
			m_entries[eventTypeIdx].push_back(std::unique_ptr<WatchedBase>(entry));
			m_idToEventType.push_back({ id, eventTypeIdx });
		}

		void RemoveEntry(WatchedId id, std::type_index eventTypeIdx) {
			auto idIt = std::find_if(m_idToEventType.begin(), m_idToEventType.end(),
				[id](const auto& pair) { return pair.first == id; });

			if (idIt != m_idToEventType.end()) {
				m_idToEventType.erase(idIt);
			}

			auto& vec = m_entries[eventTypeIdx];
			vec.erase(std::remove_if(vec.begin(), vec.end(),
				[id](const auto& entry) { return entry->m_id == id; }), vec.end());
		}

		template<typename EventT>
		void EnsureEventType() {
			static_assert(std::is_base_of_v<EventBase, EventT>, "EventT must derive from Core::Event or be void");

			auto eventTypeIdx = std::type_index(typeid(EventT));
			if (m_typeToListenerId.find(eventTypeIdx) != m_typeToListenerId.end()) {
				return;
			}

			m_entries[eventTypeIdx] = std::vector<std::unique_ptr<WatchedBase>>();

			spdlog::debug("First watched for event type: {}, adding listener", eventTypeIdx.name());
			EventListenerId listenerId = EventManager::GetInstance()->AddListener<EventT>([this](const EventT& event) {
				this->Update(std::type_index(typeid(EventT)));
			});

			m_typeToListenerId[eventTypeIdx] = listenerId;
		}

		template<>
		void EnsureEventType<void>() {
			auto eventTypeIdx = std::type_index(typeid(void));
			if (m_typeToListenerId.find(eventTypeIdx) != m_typeToListenerId.end()) {
				return;
			}

			m_entries[eventTypeIdx] = std::vector<std::unique_ptr<WatchedBase>>();

			spdlog::debug("First watched of type void, no event listener needed");
		}

		void RemoveEventType(std::type_index eventType) {
			// Note: m_entries entry is not being removed
			spdlog::debug("No more watched for event type: {}, removing listener", eventType.name());

			auto it = m_typeToListenerId.find(eventType);
			if (it == m_typeToListenerId.end()) return;
			EventManager::GetInstance()->RemoveListener(eventType, it->second);
			m_typeToListenerId.erase(it);
		}

		void ProcessPendingChanges() {
			while (!m_pendingChanges.empty()) {
				PendingChange change = m_pendingChanges.front();
				m_pendingChanges.pop();

				if (change.type == ChangeType::Add) {
					ApplyAdd(change);
				}
				else if (change.type == ChangeType::Remove) {
					ApplyRemove(change);
				}
			}
		}

		void ApplyAdd(PendingChange& change) {
			WatchedBase* entry = change.entry;
			WatchedId id = entry->m_id;
			std::type_index eventTypeIdx = change.eventType;

			spdlog::debug("Applying queued add for watched: {} with id: {} (Event: {})", 
				typeid(*entry).name(), id, eventTypeIdx.name());

			m_entries[eventTypeIdx].push_back(std::unique_ptr<WatchedBase>(entry));
			m_idToEventType.push_back({ id, eventTypeIdx });
		}

		void ApplyRemove(PendingChange& change) {
			spdlog::debug("Applying queued remove for watched with id: {}", change.id);
			RemoveEntry(change.id, change.eventType);
		}

		void Update(std::type_index eventType) {
			auto it = m_entries.find(eventType);
			if (it == m_entries.end()) return;

			if (it->second.empty()) {
				RemoveEventType(eventType);
				return;
			}

			m_updating = true;

			for (auto& entry : it->second) {
				entry->Update();
			}

			m_updating = false;
			ProcessPendingChanges();
		}

		static WatchManager* m_instance;
		std::unordered_map<std::type_index, std::vector<std::unique_ptr<WatchedBase>>> m_entries;
		std::vector<std::pair<WatchedId, std::type_index>> m_idToEventType;
		std::unordered_map<std::type_index, EventListenerId> m_typeToListenerId;
		WatchedId m_nextEntryId = 1;
		bool m_updating = false;
		std::queue<PendingChange> m_pendingChanges = {};
	};
}