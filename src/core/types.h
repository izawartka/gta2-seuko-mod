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
	template<typename T>
	using Resolver = std::function<T* ()>;

	template<typename T>
	using WatchedListener = std::function<void(std::optional<T>, std::optional<T>)>;

	template<typename T, typename U>
	using WatchedMethodListener = void (U::*)(std::optional<T>, std::optional<T>);

	using WatchedId = size_t;
}