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

        template<typename EventT, typename T>
        Watched<T>* Watch(Resolver<T> resolver, WatchedListener<T> listener)
        {
            static_assert(std::is_copy_constructible<T>::value, "ComponentT must be copy-constructible");
			static_assert(std::is_base_of_v<EventBase, EventT> || std::is_same<EventT, void>::value, "EventT must derive from Core::Event or be void");
			spdlog::debug("Adding watched: {} (Event: {})", typeid(T).name(), typeid(EventT).name());

			auto entry = new Watched<T>(m_nextEntryId++, std::move(resolver), std::move(listener));
            WatchedId id = entry->m_id;

			EnsureEventType<EventT>();

			auto eventTypeIdx = std::type_index(typeid(EventT));
			m_entries[eventTypeIdx].push_back(std::unique_ptr<WatchedBase>(entry));
            m_idToEventType.push_back({ id, eventTypeIdx });

			return entry;
        }

		template<typename EventT, typename T, typename U>
        Watched<T>* Watch(Resolver<T> resolver, U* instance, WatchedMethodListener<T, U> method) {
            static_assert(std::is_copy_constructible<T>::value, "ComponentT must be copy-constructible");
            static_assert(std::is_base_of_v<EventBase, EventT> || std::is_same<EventT, void>::value, "EventT must derive from Core::Event or be void");

            return Watch<EventT, T>(std::move(resolver), [instance, method](std::optional<T> oldValue, std::optional<T> newValue) {
                (instance->*method)(oldValue, newValue);
			});
        }

        void Unwatch(WatchedId id) {
			spdlog::debug("Removing watched with id: {}", id);

			auto it = std::find_if(m_idToEventType.begin(), m_idToEventType.end(),
				[id](const auto& pair) { return pair.first == id; });

            if (it == m_idToEventType.end()) {
				spdlog::warn("No watched found with id: {}", id);
                /// TODO
            }

			auto eventTypeIdx = it->second;
			m_idToEventType.erase(it);

			auto& vec = m_entries[eventTypeIdx];
			vec.erase(std::remove_if(vec.begin(), vec.end(),
				[id](const auto& entry) { return entry->m_id == id; }), vec.end());
        }

		template <typename T>
        void Unwatch(Watched<T>* watched) {
            Unwatch(watched->m_id);
		}

    private:
		friend class Core;
        WatchManager();
        ~WatchManager();
        WatchManager(const WatchManager&) = delete;
        WatchManager& operator=(const WatchManager&) = delete;

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

        void Update(std::type_index eventType) {
            auto it = m_entries.find(eventType);
            if (it == m_entries.end()) return;

			if (it->second.empty()) {
                RemoveEventType(eventType);
                return;
			}

			for (auto& entry : it->second) {
                entry->Update();
            }
        }

		static WatchManager* m_instance;
        std::unordered_map<std::type_index, std::vector<std::unique_ptr<WatchedBase>>> m_entries;
		std::vector<std::pair<WatchedId, std::type_index>> m_idToEventType;
        std::unordered_map<std::type_index, EventListenerId> m_typeToListenerId;
        WatchedId m_nextEntryId = 1;
    };
}