#pragma once
#include "common.h"
#include "types.h"
#include "event-base.h"

namespace Core {
	class EventManager;

	static constexpr unsigned int EVENT_MANAGER_NO_PRIORITY = std::numeric_limits<unsigned int>::max();

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
			if (m_dispatching) {
				spdlog::error("EventManager: Event dispatching is already in progress for event type: {}", typeid(EventT).name());
				return;
			}

			m_dispatching = true;

			if (m_needsSort) {
				std::stable_sort(m_listeners.begin(), m_listeners.end(),
					[](const ListenerWrapper& a, const ListenerWrapper& b) {
					return a.priority < b.priority;
				});
				m_needsSort = false;
			}

			for (auto& wrapper : m_listeners) {
				wrapper.listener(event);

				if constexpr (EventIsCancellable_v<EventT>) {
					if (event.IsCancelled()) break;
				}

				if (wrapper.isToBeRemoved) {
					spdlog::debug("EventManager: Skipping listener id {} marked for removal for event type: {}", wrapper.id, typeid(EventT).name());
					continue;
				}

				if (wrapper.isOneTime) {
					m_pendingChanges.push_back({ ChangeType::Remove, wrapper.id });
				}
			}

			m_dispatching = false;
			ProcessPendingChanges();
		}

		void AddListener(EventListener<EventT> listener, EventListenerId id, bool oneTime = false, unsigned int priority = EVENT_MANAGER_NO_PRIORITY) {
			PendingChange change{ ChangeType::Add, id, std::move(listener), oneTime, priority };
			if (m_dispatching) {
				m_pendingChanges.push_back(std::move(change));
				return;
			}
			CallInitIfNeeded();
			AddListenerEntry(std::move(change));
		}

		void RemoveListener(EventListenerId id) {
			PendingChange change = { ChangeType::Remove, id };
			if (m_dispatching) {
				m_pendingChanges.push_back(std::move(change));
				MarkListenerEntryAsToBeRemoved(id);
				return;
			}
			RemoveListenerEntry(std::move(change));
			CallDeinitIfNeeded();
		}

		struct ListenerWrapper {
			EventListenerId id;
			EventListener<EventT> listener;
			bool isOneTime = false;
			unsigned int priority = EVENT_MANAGER_NO_PRIORITY;
			bool isToBeRemoved = false;
		};

		enum class ChangeType { Add, Remove };

		struct PendingChange {
			ChangeType type;
			EventListenerId id;
			EventListener<EventT> listener = nullptr; // Only used for Add
			bool isOneTime = false; // Only used for Add
			unsigned int priority = EVENT_MANAGER_NO_PRIORITY; // used only for Add with priority
		};

		void AddListenerEntry(const PendingChange&& change) {
            spdlog::debug("EventManager: Adding listener id {} for event type: {}", change.id, typeid(EventT).name());
            m_listeners.push_back({ change.id, std::move(change.listener), change.isOneTime, change.priority, false });
			if (change.priority != EVENT_MANAGER_NO_PRIORITY) {
				m_priorityEventCount++;
			}
			if (m_priorityEventCount > 0) {
				m_needsSort = true;
			}
        }

		void MarkListenerEntryAsToBeRemoved(EventListenerId id) {
			spdlog::debug("EventManager: Marking listener id {} as to be removed for event type: {}", id, typeid(EventT).name());
			auto lit = std::find_if(m_listeners.begin(), m_listeners.end(), [id](const ListenerWrapper& wrapper) {
				return wrapper.id == id;
			});
			if (lit != m_listeners.end()) {
				spdlog::debug("EventManager: Marked listener id {} as to be removed", id);
				lit->isToBeRemoved = true;
				return;
			}

			auto pit = std::find_if(m_pendingChanges.begin(), m_pendingChanges.end(), [id](const PendingChange& change) {
				return change.type == ChangeType::Add && change.id == id;
			});
			if (pit != m_pendingChanges.end()) {
				spdlog::debug("EventManager: Listener id {} was pending addition, removing from pending changes", id);
				m_pendingChanges.erase(pit);
				return;
			}

			spdlog::warn("EventManager: Cannot mark listener id {} as to be removed. Listener not found for event type: {}", id, typeid(EventT).name());
		}

		void RemoveListenerEntry(const PendingChange&& change) {
			spdlog::debug("EventManager: Removing listener id {} for event type: {}", change.id, typeid(EventT).name());
			auto lit = std::find_if(m_listeners.begin(), m_listeners.end(), [id = change.id](const ListenerWrapper& wrapper) {
				return wrapper.id == id;
			});
			if (lit == m_listeners.end()) {
				spdlog::warn("EventManager: Cannot remove. Listener id {} not found for event type: {}", change.id, typeid(EventT).name());
				return;
			}
			if (lit->priority != EVENT_MANAGER_NO_PRIORITY) {
				m_priorityEventCount--;
			}
			m_listeners.erase(lit);
		}

		void ProcessPendingChanges() {
			if (m_pendingChanges.empty()) return;

			CallInitIfNeeded();
			while (!m_pendingChanges.empty()) {
				auto change = std::move(m_pendingChanges.front());
				m_pendingChanges.pop_front();

				switch (change.type) {
				case ChangeType::Add:
					AddListenerEntry(std::move(change));
					break;
				case ChangeType::Remove:
					RemoveListenerEntry(std::move(change));
					break;
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
		std::list<PendingChange> m_pendingChanges = {};
		bool m_initialized = false;
		bool m_hasRemoveLink = false;
		bool m_needsSort = false;
		size_t m_priorityEventCount = 0;
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
		EventListenerId AddListener(EventListener<EventT> listener, bool oneTime = false, unsigned int priority = EVENT_MANAGER_NO_PRIORITY) {
			static_assert(std::is_base_of<EventBase, EventT>::value, "EventT must derive from Core::EventBase");

			EventSpecificManager<EventT>* specificManager = EventSpecificManager<EventT>::GetInstance();
			if(specificManager->m_hasRemoveLink == false) {
				m_removalMap[typeid(EventT)] = std::bind(&EventSpecificManager<EventT>::RemoveListener, specificManager, std::placeholders::_1);
				specificManager->m_hasRemoveLink = true;
			}
			EventListenerId id = m_nextListenerId++;
			specificManager->AddListener(listener, id, oneTime, priority);
			return id;
		}

		template<typename EventT, typename U, typename Method,
			typename = std::enable_if_t<std::is_member_function_pointer_v<Method>>>
		EventListenerId AddListener(U* instance, Method method, bool oneTime = false, unsigned int priority = EVENT_MANAGER_NO_PRIORITY) {
			static_assert(std::is_base_of<EventBase, EventT>::value, "EventT must derive from Core::EventBase");
			static_assert(
				std::is_invocable_r_v<void, Method, U*, EventT&>,
				"Method must be a member function of U that takes EventT& as a parameter and returns void"
			);

			return AddListener<EventT>(
				[instance, method](EventT& e) {
					(instance->*method)(e);
				},
				oneTime,
				priority
			);
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
