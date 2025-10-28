#pragma once
#include "common.h"

namespace {
    template<typename Class, typename Member>
    struct MemStep {
        Member Class::* mptr;
        constexpr MemStep(Member Class::* p) : mptr(p) {}

        template<typename Ptr>
        auto operator()(Ptr base) const
            -> std::conditional_t<std::is_pointer<Member>::value, Member, std::add_pointer_t<Member>>
        {
            using Ret = std::conditional_t<std::is_pointer<Member>::value, Member, std::add_pointer_t<Member>>;
            if (!base) return static_cast<Ret>(nullptr);
            if constexpr (std::is_pointer<Member>::value) {
                return base->*mptr;
            }
            else {
                return std::addressof(base->*mptr);
            }
        }
    };

    template<typename Class, typename Element, std::size_t N>
    struct MemStep<Class, Element[N]> {
        Element(Class::* mptr)[N];
        constexpr MemStep(Element(Class::* p)[N]) : mptr(p) {}
        template<typename Ptr>
        Element* operator()(Ptr base) const {
            if (!base) return nullptr;
            return &((base->*mptr)[0]);
        }
    };

    template<typename Class, typename Member>
    constexpr auto mem(Member Class::* p) { return MemStep<Class, Member>(p); }

    template<typename Class, typename Member>
    struct MemAddr {
        Member Class::* mptr;
        constexpr MemAddr(Member Class::* p) : mptr(p) {}

        template<typename Ptr>
        auto operator()(Ptr base) const -> std::add_pointer_t<Member> {
            using Ret = std::add_pointer_t<Member>;
            if (!base) return static_cast<Ret>(nullptr);
            return std::addressof(base->*mptr);   // always return address of the member
        }
    };

    template<typename Class, typename Member>
    constexpr auto mem_addr(Member Class::* p) { return MemAddr<Class, Member>(p); }

    struct IdxStep {
        std::size_t idx;
        constexpr IdxStep(std::size_t i) : idx(i) {}

        template<typename ArrPtr>
        auto operator()(ArrPtr arr) const {
            using Elem = std::remove_pointer_t<std::decay_t<ArrPtr>>;
            using Ret = std::conditional_t<std::is_pointer<Elem>::value, Elem, std::add_pointer_t<Elem>>;
            if (!arr) return static_cast<Ret>(nullptr);
            if constexpr (std::is_pointer<Elem>::value) {
                return static_cast<Ret>(arr[idx]);
            }
            else {
                return static_cast<Ret>(std::addressof(arr[idx]));
            }
        }
    };
    inline constexpr IdxStep idx(std::size_t i) { return IdxStep(i); }

    // TupleStep: applies multiple steps to the same base and returns a tuple of results
    template<typename... InnerSteps>
    struct TupleStep {
        std::tuple<InnerSteps...> steps;
        constexpr TupleStep(InnerSteps... s) : steps(s...) {}

        template<typename Ptr>
        auto operator()(Ptr base) const {
            return std::apply([&](auto const&... s) {
                return std::make_tuple(s(base)...);
            }, steps);
        }
    };

    template<typename... InnerSteps>
    constexpr auto tupl(InnerSteps... steps) { return TupleStep<InnerSteps...>(steps...); }

    template<std::size_t I, typename Cur, typename Tuple>
    auto ChainStep(Cur cur, const Tuple& t)
    {
        if constexpr (I >= std::tuple_size_v<Tuple>) {
            return cur;
        }
        else {
            auto next = std::get<I>(t)(cur);
            return ChainStep<I + 1>(next, t);
        }
    }
}

namespace Core {
    template<typename BaseGetter, typename... Steps>
    auto MakeResolver(BaseGetter baseGetter, Steps... steps)
        -> std::function< decltype(ChainStep<0>(std::declval<std::invoke_result_t<BaseGetter>>(), std::declval<std::tuple<Steps...>>()))() >
    {
        auto stepsTuple = std::make_tuple(steps...);
        using BasePtr = std::invoke_result_t<BaseGetter>;
        using FinalPtr = decltype(ChainStep<0>(std::declval<BasePtr>(), stepsTuple));

        return std::function<FinalPtr()>(
            [baseGetter = std::move(baseGetter), stepsTuple]() -> FinalPtr {
            BasePtr base = baseGetter();
            return ChainStep<0>(base, stepsTuple);
        });
    }
}
