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

			if (HasEventListener<EventT>()) {
				spdlog::error("Event listener for event type {} already added", eventTypeIdx.name());
				return false;
			}

			return InternalAddEventListener<EventT, U>(method, oneTime);
		}

		template<typename EventT, typename U>
		void SetEventListener(EventMethodListener<EventT, U> method, bool enabled) {
			static_assert(std::is_base_of<EventBase, EventT>::value, "EventT must derive from Core::EventBase");

			if (enabled) {
				if (!HasEventListener<EventT>()) {
					InternalAddEventListener<EventT, U>(method);
				}
			}
			else {
				auto it = FindEventListener<EventT>();
				if (it != m_eventListenerIds.end()) {
					InternalRemoveEventListener<EventT>(it);
				}
			}
		}

		template<typename EventT>
		void RemoveEventListener(bool failSilently = false) {
			static_assert(std::is_base_of<EventBase, EventT>::value, "EventT must derive from Core::EventBase");
			const std::type_index eventTypeIdx = typeid(EventT);

			auto it = FindEventListener<EventT>();
			if (it == m_eventListenerIds.end()) {
				if (!failSilently) spdlog::warn("No event listener found for event type {}", eventTypeIdx.name());
				return;
			}

			InternalRemoveEventListener<EventT>(it);
		}

		template<typename EventT>
		bool HasEventListener() const {
			static_assert(std::is_base_of<EventBase, EventT>::value, "EventT must derive from Core::EventBase");
			const std::type_index eventTypeIdx = typeid(EventT);

			return FindEventListener<EventT>() != m_eventListenerIds.end();
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
		template<typename EventT>
		std::unordered_map<std::type_index, EventListenerId>::iterator FindEventListener() {
			const std::type_index eventTypeIdx = typeid(EventT);
			return m_eventListenerIds.find(eventTypeIdx);
		}

		template<typename EventT>
		std::unordered_map<std::type_index, EventListenerId>::const_iterator FindEventListener() const {
			const std::type_index eventTypeIdx = typeid(EventT);
			return m_eventListenerIds.find(eventTypeIdx);
		}

		template<typename EventT, typename U>
		bool InternalAddEventListener(EventMethodListener<EventT, U> method, bool oneTime = false) {
			const std::type_index eventTypeIdx = typeid(EventT);
			EventListenerId listenerId = 0;
			auto* eventManager = EventManager::GetInstance();

			if (oneTime) {
				// create a lambda that removes the listener from m_eventListenerIds after first call
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
		void InternalRemoveEventListener(std::unordered_map<std::type_index, EventListenerId>::iterator it) {
			auto* eventManager = EventManager::GetInstance();
			eventManager->RemoveListener<EventT>(it->second);
			m_eventListenerIds.erase(it);
		}

		std::unordered_map<std::type_index, EventListenerId> m_eventListenerIds;
	};
}
