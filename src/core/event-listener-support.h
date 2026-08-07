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

        template<typename EventT, typename U, typename Method,
            typename = std::enable_if_t<std::is_member_function_pointer_v<Method>>>
		bool AddEventListener(Method method, bool oneTime = false, unsigned int priority = EVENT_MANAGER_NO_PRIORITY) {
            static_assert(
                std::is_base_of<EventBase, EventT>::value,
                "EventT must derive from Core::EventBase"
            );
            static_assert(
                std::is_invocable_r_v<void, Method, U*, EventT&>,
				"Method must be invocable with an argument of type EventT& and return void"
            );

            const std::type_index eventTypeIdx = typeid(EventT);

            if (HasEventListener<EventT>()) {
                spdlog::error("Event listener for event type {} already added", eventTypeIdx.name());
                return false;
            }

            return InternalAddEventListener<EventT, U>(method, oneTime, priority);
        }

        template<typename EventT, typename U>
        bool AddEventListener(void (U::* method)(EventT&), bool oneTime = false, unsigned int priority = EVENT_MANAGER_NO_PRIORITY) {
			return AddEventListener<EventT, U, decltype(method)>(method, oneTime, priority);
		}

		template<typename EventT, typename U>
        bool AddEventListener(void (U::* method)(const EventT&), bool oneTime = false, unsigned int priority = EVENT_MANAGER_NO_PRIORITY) {
            return AddEventListener<EventT, U, decltype(method)>(method, oneTime, priority);
        }

		template<typename EventT, typename U>
		bool AddEventListener(void (U::* method)(EventT&) const, bool oneTime = false, unsigned int priority = EVENT_MANAGER_NO_PRIORITY) {
			return AddEventListener<EventT, U, decltype(method)>(method, oneTime, priority);
		}

		template<typename EventT, typename U>
		bool AddEventListener(void (U::* method)(const EventT&) const, bool oneTime = false, unsigned int priority = EVENT_MANAGER_NO_PRIORITY) {
			return AddEventListener<EventT, U, decltype(method)>(method, oneTime, priority);
		}

        template<typename EventT, typename U, typename Method,
            typename = std::enable_if_t<std::is_member_function_pointer_v<Method>>>
		void SetEventListener(Method method, bool enabled, unsigned int priority = EVENT_MANAGER_NO_PRIORITY) {
            static_assert(
                std::is_base_of<EventBase, EventT>::value,
                "EventT must derive from Core::EventBase"
            );
            static_assert(
                std::is_invocable_r_v<void, Method, U*, EventT&>,
				"Method must be invocable with an argument of type EventT& and return void"
            );

            if (enabled) {
                if (!HasEventListener<EventT>()) {
                    InternalAddEventListener<EventT, U>(method, false, priority);
                }
            }
            else {
                auto it = FindEventListener<EventT>();
                if (it != m_eventListenerIds.end()) {
                    InternalRemoveEventListener<EventT>(it);
                }
            }
        }

		template<typename EventT, typename U>
		void SetEventListener(void (U::* method)(EventT&), bool enabled, unsigned int priority = EVENT_MANAGER_NO_PRIORITY) {
			SetEventListener<EventT, U, decltype(method)>(method, enabled, priority);
		}

		template<typename EventT, typename U>
		void SetEventListener(void (U::* method)(const EventT&), bool enabled, unsigned int priority = EVENT_MANAGER_NO_PRIORITY) {
			SetEventListener<EventT, U, decltype(method)>(method, enabled, priority);
		}

        template<typename EventT, typename U>
        void SetEventListener(void (U::* method)(EventT&) const, bool enabled, unsigned int priority = EVENT_MANAGER_NO_PRIORITY) {
            SetEventListener<EventT, U, decltype(method)>(method, enabled, priority);
        }

        template<typename EventT, typename U>
        void SetEventListener(void (U::* method)(const EventT&) const, bool enabled, unsigned int priority = EVENT_MANAGER_NO_PRIORITY) {
            SetEventListener<EventT, U, decltype(method)>(method, enabled, priority);
        }

        template<typename EventT>
        void RemoveEventListener(bool failSilently = false) {
            static_assert(
                std::is_base_of<EventBase, EventT>::value, 
                "EventT must derive from Core::EventBase"
            );

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
            static_assert(
                std::is_base_of<EventBase, EventT>::value,
                "EventT must derive from Core::EventBase"
            );

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

        template<typename EventT, typename U, typename Method>
        bool InternalAddEventListener(Method method, bool oneTime = false, unsigned int priority = EVENT_MANAGER_NO_PRIORITY) {
            const std::type_index eventTypeIdx = typeid(EventT);
            EventListenerId listenerId = 0;
            auto* eventManager = EventManager::GetInstance();

            if (oneTime) {
                // create a lambda that removes the listener from m_eventListenerIds on first call
                listenerId = eventManager->template AddListener<EventT>(
                    [this, method, eventTypeIdx](EventT& event) {
                    auto it = m_eventListenerIds.find(eventTypeIdx);
                    if (it != m_eventListenerIds.end()) {
                        m_eventListenerIds.erase(it);
                    }
                    (static_cast<U*>(this)->*method)(event);
                },
                    true,
                    priority
                );
            }
            else {
                listenerId = eventManager->template AddListener<EventT>(
                    static_cast<U*>(this), method, false, priority
                );
            }

            if (listenerId == 0) return false;

            m_eventListenerIds[eventTypeIdx] = listenerId;
            return true;
        }

        template<typename EventT>
        void InternalRemoveEventListener(
            std::unordered_map<std::type_index, EventListenerId>::iterator it) {
            auto* eventManager = EventManager::GetInstance();
            eventManager->RemoveListener<EventT>(it->second);
            m_eventListenerIds.erase(it);
        }

        std::unordered_map<std::type_index, EventListenerId> m_eventListenerIds;
    };
}
