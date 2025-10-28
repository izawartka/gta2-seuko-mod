#pragma once
#include "types.h"
#include "common.h"

namespace Core 
{
    class WatchedBase 
    {
    public:
        virtual ~WatchedBase() = default;
        virtual void Update() = 0;
    protected:
        friend class WatchManager;
        WatchedBase() = default;
        WatchedId m_id = 0;
    };

    template<typename T>
    class Watched : public WatchedBase {
    public:
        T* GetPointer() {
            return m_resolver();
        }

        std::optional<T> GetValue() {
            T* p = m_resolver();
            if (p) return *p;
            return std::nullopt;
        }

        bool SetValue(T value) {
            T* p = m_resolver();
            if (p) {
                *p = value;
                return true;
            }
            return false;
        }

        void Update() override {
            T* p = m_resolver();
            bool present = (p != nullptr);
            std::optional<T> value = std::nullopt;
            if (present) value = *p;

            if (m_needsUpdate || value != m_lastValue) {
                std::optional<T> lastValue = m_lastValue;
                m_needsUpdate = false;
                m_lastValue = value;
                m_listener(lastValue, value);
            }
        }

        WatchedId GetId() const {
            return m_id;
        }

        void RequestUpdate() {
            m_needsUpdate = true;
        }

    private:
        friend class WatchManager;

        Watched(WatchedId id_, std::function<T* ()> r, std::function<void(std::optional<T>, std::optional<T>)> l)
            : m_resolver(std::move(r)), m_listener(std::move(l))
        {
            m_id = id_;
        }

        Watched(const Watched&) = delete;
        Watched& operator=(const Watched&) = delete;

        std::function<T* ()> m_resolver;
        std::function<void(std::optional<T>, std::optional<T>)> m_listener;
        std::optional<T> m_lastValue;
        bool m_needsUpdate = true;
    };

    // Partial specialization for tuple types where resolver returns a tuple of pointers
    template<typename... Values>
    class Watched<std::tuple<Values...>> : public WatchedBase {
    public:
        using Tuple = std::tuple<Values...>;
        using PtrTuple = std::tuple<Values*...>;

        Watched(WatchedId id_, std::function<PtrTuple()> resolver, std::function<void(std::optional<Tuple>, std::optional<Tuple>)> listener)
            : m_resolver(std::move(resolver)), m_listener(std::move(listener))
        {
            m_id = id_;
        }

        // No GetPointer for tuple (no stable pointer to tuple exists)

        std::optional<Tuple> GetValue() {
            PtrTuple ptrs = m_resolver();
            if (any_null(ptrs)) return std::nullopt;
            return std::optional<Tuple>(deref_tuple(ptrs));
        }

        bool SetValue(Tuple value) {
            PtrTuple ptrs = m_resolver();
            if (any_null(ptrs)) return false;
            assign_tuple(ptrs, value);
            return true;
        }

        void Update() override {
            PtrTuple ptrs = m_resolver();
            bool present = !any_null(ptrs);
            std::optional<Tuple> value = std::nullopt;
            if (present) value = deref_tuple(ptrs);

            if (m_needsUpdate || value != m_lastValue) {
                std::optional<Tuple> lastValue = m_lastValue;
                m_needsUpdate = false;
                m_lastValue = value;
                m_listener(lastValue, value);
            }
        }

        WatchedId GetId() const { return m_id; }
        void RequestUpdate() { m_needsUpdate = true; }

    private:
        friend class WatchManager;

        template<std::size_t... I>
        static Tuple deref_impl(const PtrTuple& ptrs, std::index_sequence<I...>) {
            return Tuple{ ( *std::get<I>(ptrs) )... };
        }

        static Tuple deref_tuple(const PtrTuple& ptrs) {
            return deref_impl(ptrs, std::index_sequence_for<Values...>{});
        }

        template<std::size_t... I>
        static bool any_null_impl(const PtrTuple& ptrs, std::index_sequence<I...>) {
            bool any = false;
            (void)std::initializer_list<int>{ (any = any || (std::get<I>(ptrs) == nullptr), 0)... };
            return any;
        }

        static bool any_null(const PtrTuple& ptrs) {
            return any_null_impl(ptrs, std::index_sequence_for<Values...>{});
        }

        template<std::size_t... I>
        static void assign_impl(const PtrTuple& ptrs, const Tuple& value, std::index_sequence<I...>) {
            (void)std::initializer_list<int>{ ( ( *std::get<I>(ptrs) ) = std::get<I>(value), 0 )... };
        }

        static void assign_tuple(const PtrTuple& ptrs, const Tuple& value) {
            assign_impl(ptrs, value, std::index_sequence_for<Values...>{});
        }

        std::function<PtrTuple()> m_resolver;
        std::function<void(std::optional<Tuple>, std::optional<Tuple>)> m_listener;
        std::optional<Tuple> m_lastValue;
        bool m_needsUpdate = true;
    };
}