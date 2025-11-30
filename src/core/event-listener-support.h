#pragma once
#include "common.h"
#include "types.h"
#include "event-base.h"
#include "event-manager.h"

namespace Core {

	class EventListenerSupport {
	protected:
		EventListenerSupport() = default;

		virtual ~EventListenerSupport() {
			RemoveAllEventListeners();
		}

		template<typename EventT, typename U>
		bool AddEventListener(EventMethodListener<EventT, U> method, bool oneTime = false) {
			static_assert(std::is_base_of<EventBase, EventT>::value, "EventT must derive from Core::EventBase");
			const std::type_index eventTypeIdx = typeid(EventT);

			if (m_eventListenerIds.find(eventTypeIdx) != m_eventListenerIds.end()) {
				spdlog::error("Event listener for event type {} already added", eventTypeIdx.name());
				return false;
			}

			EventListenerId listenerId = 0;
			auto* eventManager = EventManager::GetInstance();
			if (oneTime) {
				// create lambda that removes the listener from m_eventListenerIds after first call
				listenerId = eventManager->AddListener<EventT>([this, method, eventTypeIdx](EventT& event) {
					auto it = m_eventListenerIds.find(eventTypeIdx);
					if (it != m_eventListenerIds.end()) {
						m_eventListenerIds.erase(it);
					}
					(static_cast<U*>(this)->*method)(event);
				}, true);
			}
			else {
				listenerId = eventManager->AddListener<EventT>(static_cast<U*>(this), method);
			}

			if (listenerId == 0) return false;

			m_eventListenerIds[eventTypeIdx] = listenerId;
			return true;
		}

		template<typename EventT>
		void RemoveEventListener() {
			static_assert(std::is_base_of<EventBase, EventT>::value, "EventT must derive from Core::EventBase");
			const std::type_index eventTypeIdx = typeid(EventT);

			auto it = m_eventListenerIds.find(eventTypeIdx);
			if (it == m_eventListenerIds.end()) {
				spdlog::warn("No event listener found for event type {}", eventTypeIdx.name());
				return;
			}
			auto* eventManager = EventManager::GetInstance();
			eventManager->RemoveListener<EventT>(it->second);
			m_eventListenerIds.erase(it);
		}

		template<typename EventT>
		bool HasEventListener() {
			static_assert(std::is_base_of<EventBase, EventT>::value, "EventT must derive from Core::EventBase");
			const std::type_index eventTypeIdx = typeid(EventT);

			auto it = m_eventListenerIds.find(eventTypeIdx);
			return (it != m_eventListenerIds.end());
		}

		void RemoveAllEventListeners() {
			auto* eventManager = EventManager::GetInstance();
			for (const auto& pair : m_eventListenerIds) {
				eventManager->RemoveListener(pair.first, pair.second);
			}
			m_eventListenerIds.clear();
		}

		EventListenerSupport(const EventListenerSupport&) = delete;
		EventListenerSupport& operator=(const EventListenerSupport&) = delete;
		EventListenerSupport(EventListenerSupport&&) = delete;
		EventListenerSupport& operator=(EventListenerSupport&&) = delete;

	private:
		std::unordered_map<std::type_index, EventListenerId> m_eventListenerIds;
	};
}
