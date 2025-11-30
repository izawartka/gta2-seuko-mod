#pragma once

namespace Core
{
	template<typename EventT, typename = void>
	struct EventHasInit : std::false_type {};

	template<typename EventT>
	struct EventHasInit<EventT, std::void_t<decltype(EventT::Init())>> : std::true_type {};

	template<typename EventT>
	static constexpr bool EventHasInit_v = EventHasInit<EventT>::value;

	class EventBase
	{
	protected:
		EventBase() = default;
		virtual ~EventBase() = default;
	};
}
