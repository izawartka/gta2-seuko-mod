#pragma once
#include "common.h"
#include "types.h"
#include "event-base.h"

namespace Core
{
	class EventManager
	{
	public:
		static EventManager* GetInstance();

		template<typename EventT>
		void Dispatch(EventT& event) {
			static_assert(std::is_base_of<EventBase, EventT>::value, "EventT must derive from Core::EventBase");

			auto it = m_listeners.find(typeid(EventT));
			if (it == m_listeners.end()) return;

			m_dispatching = true;
			auto& listeners = it->second;
			for (auto& wrapper : listeners) {
				wrapper.listener(event);
				if (wrapper.isOneTime) {
					RemoveListenerEntry({ ChangeType::Remove, typeid(EventT), wrapper.id });
				}
			}
			m_dispatching = false;
			ProcessPendingChanges();
		}

		template<typename EventT>
		EventListenerId AddListener(EventListener<EventT> listener, bool oneTime = false) {
			static_assert(std::is_base_of<EventBase, EventT>::value, "EventT must derive from Core::EventBase");
			std::type_index eventTypeIdx = typeid(EventT);

			auto it = m_listeners.find(eventTypeIdx);
			if (it == m_listeners.end() && !callInit<EventT>()) {
				spdlog::error("Event type {} failed to initialize, listener not added", eventTypeIdx.name());
				return 0;
			}

			EventListenerId id = m_nextListenerId++;
			PendingChange change = { ChangeType::Add, eventTypeIdx, id, [listener](EventBase& e) {
				listener(static_cast<EventT&>(e));
			}, oneTime };
			if (m_dispatching) {
				m_pendingChanges.push(change);
				return id;
			}
			AddListenerEntry(change);
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
			PendingChange change = { ChangeType::Remove, eventType, id };
			if (m_dispatching) {
				m_pendingChanges.push(change);
				return;
			}
			RemoveListenerEntry(change);
		}

		template<typename EventT>
		void RemoveListener(EventListenerId id) {
			static_assert(std::is_base_of<EventBase, EventT>::value, "EventT must derive from Core::EventBase");
			RemoveListener(typeid(EventT), id);
		}

	private:
		friend class Core;
		EventManager();
		~EventManager();
		EventManager(const EventManager&) = delete;
		EventManager& operator=(const EventManager&) = delete;

		static EventManager* m_instance;

		struct ListenerWrapper {
			EventListenerId id;
			EventListener<EventBase> listener;
			bool isOneTime = false;
		};

		enum class ChangeType { Add, Remove };
		struct PendingChange {
			ChangeType type;
			std::type_index eventType;
			EventListenerId id;
			EventListener<EventBase> listener = nullptr; // Only used for Add
			bool isOneTime = false; // Only used for Add
		};

		void AddListenerEntry(PendingChange change) {
			spdlog::debug("Adding listener id {} for event type: {}", change.id, change.eventType.name());
			auto& listeners = m_listeners[change.eventType];
			listeners.push_back({ change.id, change.listener, change.isOneTime });
		}

		void RemoveListenerEntry(PendingChange change) {
			spdlog::debug("Removing listener id {} for event type: {}", change.id, change.eventType.name());
			auto it = m_listeners.find(change.eventType);
			if (it == m_listeners.end()) {
				spdlog::warn("No listeners found for event type: {}", change.eventType.name());
				return;
			}
			auto& listeners = it->second;
			auto lit = std::find_if(listeners.begin(), listeners.end(), [id = change.id](const ListenerWrapper& wrapper) {
				return wrapper.id == id;
			});
			if (lit == listeners.end()) {
				spdlog::warn("Listener id {} not found for event type: {}", change.id, change.eventType.name());
				return;
			}
			listeners.erase(lit);
			// Note: m_listeners entry is not being removed 
		}

		void ProcessPendingChanges() {
			while (!m_pendingChanges.empty()) {
				auto change = m_pendingChanges.front();
				m_pendingChanges.pop();

				if (change.type == ChangeType::Add) {
					AddListenerEntry(change);
				}
				
				if (change.type == ChangeType::Remove) {
					RemoveListenerEntry(change);
				}
			}
		}

		std::unordered_map<std::type_index, std::vector<ListenerWrapper>> m_listeners;
		EventListenerId m_nextListenerId = 1;
		bool m_dispatching = false;
		std::queue<PendingChange> m_pendingChanges;

		template<typename T, typename = void>
		struct has_init : std::false_type {};

		template<typename T>
		struct has_init<T, std::void_t<decltype(T::Init())>> : std::true_type {};

		template<typename EventT>
		bool callInit() {
			if constexpr (has_init<EventT>::value) {
				spdlog::debug("Calling Init for event type: {}", typeid(EventT).name());
				return EventT::Init();
			}
			else {
				return true;
			}
		}
	};
}
