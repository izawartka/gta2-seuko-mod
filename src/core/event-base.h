#pragma once

namespace Core
{
	class CancellableEventBase;

	template<typename EventT, typename = void>
	struct EventHasInit : std::false_type {};

	template<typename EventT>
	struct EventHasInit<EventT, std::void_t<decltype(EventT::Init())>> : std::true_type {};

	template<typename EventT>
	static constexpr bool EventHasInit_v = EventHasInit<EventT>::value;

	template<typename EventT, typename = void>
	struct EventHasDeinit : std::false_type {};

	template<typename EventT>
	struct EventHasDeinit<EventT, std::void_t<decltype(EventT::Deinit())>> : std::true_type {};

	template<typename EventT>
	static constexpr bool EventHasDeinit_v = EventHasDeinit<EventT>::value;

	template<typename EventT>
	using EventIsCancellable = std::is_base_of<CancellableEventBase, EventT>;

	template<typename EventT>
	constexpr bool EventIsCancellable_v = EventIsCancellable<EventT>::value;

	// Base class for all events. Not to be used directly.
	class EventBase
	{
	protected:
		EventBase() = default;
		virtual ~EventBase() = default;
	};

	// Base class for events that can be cancelled. Not to be used directly.
	class CancellableEventBase : public EventBase
	{
	protected:
		CancellableEventBase() = default;
		virtual ~CancellableEventBase() = default;

	public:
		virtual void Cancel() final { m_cancelled = true; }
		virtual bool IsCancelled() const final { return m_cancelled; }

	private:
		bool m_cancelled = false;
	};
}
