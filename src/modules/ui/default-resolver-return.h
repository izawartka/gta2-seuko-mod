#pragma once
#include "common.h"

namespace UiModule {
    template<typename T>
    struct DefaultResolverReturn { using type = T*; };

    template<typename... Values>
    struct DefaultResolverReturn<std::tuple<Values...>> { using type = std::tuple<Values*...>; };
}