#pragma once
#include "common.h"

namespace Core
{
	/* EventManager */
	template<typename EventT>
	using EventListener = std::function<void(EventT&)>;

	template<typename EventT, typename U>
	using EventMethodListener = void (U::*)(EventT&);

	using EventListenerId = size_t;

	/* WatchManager */
	template<typename ResRetT>
	using Resolver = std::function<ResRetT()>;

	template<typename ValueT>
	struct DefaultResRetT { using type = ValueT*; };

	template<typename... Values>
	struct DefaultResRetT<std::tuple<Values...>> { using type = std::tuple<Values*...>; };

	template<typename ValueT>
	using WatchedListener = std::function<void(std::optional<ValueT>, std::optional<ValueT>)>;

	template<typename ValueT, typename U>
	using WatchedMethodListener = void (U::*)(std::optional<ValueT>, std::optional<ValueT>);

	using WatchedId = size_t;
}