#pragma once
#include "types.h"

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

            if (m_first || value != m_lastValue) {
                m_listener(m_lastValue, value);
                m_first = false;
                m_lastValue = value;
            }
        }

        WatchedId GetId() const {
            return m_id;
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
        bool m_first = true;
    };
}