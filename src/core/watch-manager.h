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

			EnsureEventType<EventT>();

			auto entry = new Watched<ValueT, ResRetT>(m_nextEntryId++, std::move(resolver), std::move(listener));
			
			if (m_updating) {
				PendingChange change(ChangeType::Add, entry->m_id, typeid(EventT), entry);
				m_pendingChanges.push(change);
			}
			else {
				AddEntry(entry, typeid(EventT));
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

		void Unwatch(WatchedBase* watched);

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
			std::type_index eventType; // Only used for Add
			WatchedBase* entry; // Only used for Add
			
			PendingChange(ChangeType t, WatchedId i, std::type_index et, WatchedBase* e)
				: type(t), id(i), eventType(et), entry(e) {}
		};

		template<typename EventT>
		void EnsureEventType() {
			static_assert(std::is_base_of_v<EventBase, EventT>, "EventT must derive from Core::Event or be void");

			auto eventType = std::type_index(typeid(EventT));
			if (m_typeToListenerId.find(eventType) != m_typeToListenerId.end()) return;

			m_entries.insert({ eventType, std::vector<std::unique_ptr<WatchedBase>>() });

			spdlog::debug("First watched for event type: {}, adding listener", eventType.name());

			EventListenerId listenerId = EventManager::GetInstance()->AddListener<EventT>([this](const EventT& event) {
				this->Update(std::type_index(typeid(EventT)));
			});

			m_typeToListenerId.insert({ eventType, listenerId });
		}

		template<>
		void EnsureEventType<void>() {
			auto eventTypeIdx = std::type_index(typeid(void));
			if (m_typeToListenerId.find(eventTypeIdx) != m_typeToListenerId.end()) return;

			m_entries.insert({ eventTypeIdx, std::vector<std::unique_ptr<WatchedBase>>() });

			spdlog::debug("First watched of type void, no event listener needed");
		}

		void AddEntry(WatchedBase* entry, std::type_index eventType);
		void RemoveEntry(WatchedId id);
		void RemoveEventType(std::type_index eventType);
		void ProcessPendingChanges();
		void Update(std::type_index eventType);

		static WatchManager* m_instance;
		std::unordered_map<std::type_index, std::vector<std::unique_ptr<WatchedBase>>> m_entries;
		std::unordered_map<WatchedId, std::type_index> m_idToEventType;
		std::unordered_map<std::type_index, EventListenerId> m_typeToListenerId;
		WatchedId m_nextEntryId = 1;
		bool m_updating = false;
		std::queue<PendingChange> m_pendingChanges = {};
	};
}