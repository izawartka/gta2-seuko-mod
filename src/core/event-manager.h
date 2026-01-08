#pragma once
#include "common.h"
#include "types.h"
#include "event-base.h"

namespace Core {
	class EventManager;

	template<typename EventT>
	class EventSpecificManager
	{
		friend class EventManager;

		EventSpecificManager() = default;
		~EventSpecificManager() = default;
		EventSpecificManager(const EventSpecificManager&) = delete;
		EventSpecificManager& operator=(const EventSpecificManager&) = delete;

		static EventSpecificManager<EventT>* GetInstance() {
			static EventSpecificManager<EventT> instance;
			return &instance;
		}

		void Dispatch(EventT& event) {
			m_dispatching = true;
			for (auto& wrapper : m_listeners) {
				wrapper.listener(event);
				if (wrapper.isOneTime) {
					m_pendingChanges.push({ ChangeType::Remove, wrapper.id });
				}
			}
			m_dispatching = false;
			ProcessPendingChanges();
		}

		void AddListener(EventListener<EventT> listener, EventListenerId id, bool oneTime) {
			PendingChange change = { ChangeType::Add, id, std::move(listener), oneTime };
			if (m_dispatching) {
				m_pendingChanges.push(std::move(change));
				return;
			}
			CallInitIfNeeded();
			AddListenerEntry(std::move(change));
		}

		void RemoveListener(EventListenerId id) {
			PendingChange change = { ChangeType::Remove, id };
			if (m_dispatching) {
				m_pendingChanges.push(std::move(change));
				return;
			}
			RemoveListenerEntry(change);
			CallDeinitIfNeeded();
		}

		struct ListenerWrapper {
			EventListenerId id;
			EventListener<EventT> listener;
			bool isOneTime = false;
		};

		enum class ChangeType { Add, Remove };

		struct PendingChange {
			ChangeType type;
			EventListenerId id;
			EventListener<EventT> listener = nullptr; // Only used for Add
			bool isOneTime = false; // Only used for Add
		};

		void AddListenerEntry(PendingChange&& change) {
			spdlog::debug("Adding listener id {} for event type: {}", change.id, typeid(EventT).name());
			m_listeners.push_back({ change.id, std::move(change.listener), change.isOneTime });
		}

		void RemoveListenerEntry(const PendingChange& change) {
			spdlog::debug("Removing listener id {} for event type: {}", change.id, typeid(EventT).name());
			auto lit = std::find_if(m_listeners.begin(), m_listeners.end(), [id = change.id](const ListenerWrapper& wrapper) {
				return wrapper.id == id;
			});
			if (lit == m_listeners.end()) {
				spdlog::warn("Listener id {} not found for event type: {}", change.id, typeid(EventT).name());
				return;
			}
			m_listeners.erase(lit);
		}

		void ProcessPendingChanges() {
			CallInitIfNeeded();
			while (!m_pendingChanges.empty()) {
				auto change = std::move(m_pendingChanges.front());
				m_pendingChanges.pop();
				if (change.type == ChangeType::Add) {
					AddListenerEntry(std::move(change));
				}
				
				if (change.type == ChangeType::Remove) {
					RemoveListenerEntry(change);
				}
			}
			CallDeinitIfNeeded();
		}

		bool CallInitIfNeeded() {
			if (m_initialized) return true;

			if constexpr (EventHasInit_v<EventT>) {
				spdlog::debug("Calling Init for event type: {}", typeid(EventT).name());
				m_initialized = EventT::Init();
				if (!m_initialized) {
					spdlog::error("Event Init failed for event type: {}", typeid(EventT).name());
				}
			}
			else {
				m_initialized = true;
			}

			return m_initialized;
		}

		void CallDeinitIfNeeded() {
			if (!m_initialized || !m_listeners.empty()) return;

			if constexpr (EventHasDeinit_v<EventT>) {
				spdlog::debug("Calling Deinit for event type: {}", typeid(EventT).name());
				EventT::Deinit();
				m_initialized = false;
			}
		}

		std::vector<ListenerWrapper> m_listeners = {};
		bool m_dispatching = false;
		std::queue<PendingChange> m_pendingChanges = {};
		bool m_initialized = false;
		bool m_hasRemoveLink = false;
	};

	class EventManager
	{
	public:
		static EventManager* GetInstance();

		template<typename EventT>
		void Dispatch(EventT& event) {
			static_assert(std::is_base_of<EventBase, EventT>::value, "EventT must derive from Core::EventBase");
			EventSpecificManager<EventT>::GetInstance()->Dispatch(event);
		}

		template<typename EventT>
		EventListenerId AddListener(EventListener<EventT> listener, bool oneTime = false) {
			static_assert(std::is_base_of<EventBase, EventT>::value, "EventT must derive from Core::EventBase");
			EventSpecificManager<EventT>* specificManager = EventSpecificManager<EventT>::GetInstance();
			if(specificManager->m_hasRemoveLink == false) {
				m_removalMap[typeid(EventT)] = std::bind(&EventSpecificManager<EventT>::RemoveListener, specificManager, std::placeholders::_1);
				specificManager->m_hasRemoveLink = true;
			}
			EventListenerId id = m_nextListenerId++;
			specificManager->AddListener(listener, id, oneTime);
			return id;
		}

		template<typename EventT, typename U>
		EventListenerId AddListener(U* instance, EventMethodListener<EventT, U> method, bool oneTime = false) {
			static_assert(std::is_base_of<EventBase, EventT>::value, "EventT must derive from Core::EventBase");
			return AddListener<EventT>([instance, method](EventT& e) {
				(instance->*method)(e);
			}, oneTime);
		}

		void RemoveListener(std::type_index eventType, EventListenerId id) {
			auto it = m_removalMap.find(eventType);
			if (it != m_removalMap.end()) {
				it->second(id);
			}
			else {
				spdlog::error("Cannot remove listener id {}: Event of type {} has never been added", id, eventType.name());
			}
		}

		template<typename EventT>
		void RemoveListener(EventListenerId id) {
			static_assert(std::is_base_of<EventBase, EventT>::value, "EventT must derive from Core::EventBase");
			EventSpecificManager<EventT>::GetInstance()->RemoveListener(id);
		}

	private:
		friend class Core;
		EventManager();
		~EventManager();
		EventManager(const EventManager&) = delete;
		EventManager& operator=(const EventManager&) = delete;

		static EventManager* m_instance;
		EventListenerId m_nextListenerId = 1;

		std::unordered_map<std::type_index, std::function<void(EventListenerId)>> m_removalMap;
	};
}
