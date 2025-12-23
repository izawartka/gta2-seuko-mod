#pragma once
#include "types.h"
#include "common.h"

namespace Core 
{
	class WatchedBase 
	{
	public:
		virtual ~WatchedBase() = default;
		WatchedBase(const WatchedBase&) = delete;
		WatchedBase& operator=(const WatchedBase&) = delete;
		WatchedBase(WatchedBase&&) = delete;
		WatchedBase& operator=(WatchedBase&&) = delete;

		virtual void Update() = 0;
		virtual WatchedId GetId() const final { return m_id; }
		virtual void RequestUpdate() final { m_needsUpdate = true; }
	protected:
		friend class WatchManager;
		WatchedBase() = default;
		WatchedId m_id = 0;
		bool m_needsUpdate = true;
	};

	template <typename ValueT, typename ResRetT = typename DefaultResRetT<ValueT>::type>
	class Watched : public WatchedBase {};

	template<typename WatchedT>
	struct WatchedCapabilities;

	template<typename WatchedT>
	static constexpr bool WatchedHasSetValue_v = WatchedCapabilities<WatchedT>::has_set_value;

	// Specialization for value types
	template<typename T>
	class Watched<T, T> : public WatchedBase {
	public:
		Resolver<T> GetResolver() {
			return m_resolver;
		}

		const std::optional<T>& GetSavedValue() const {
			return m_savedValue;
		}

		// No SetValue for value types (read-only)

		void Update() override {
			T value = m_resolver();

			bool changed = !m_savedValue.has_value() || (value != m_savedValue.value());

			if (m_needsUpdate || changed) {
				std::optional<T> lastValue = m_savedValue;
				m_needsUpdate = false;
				m_savedValue = value;
				m_listener(lastValue, m_savedValue);
			}
		}

	private:
		friend class WatchManager;

		Watched(WatchedId id, Resolver<T> resolver, WatchedListener<T> listener)
			: m_resolver(std::move(resolver)), m_listener(std::move(listener)) {
			m_id = id;
		}

		Resolver<T> m_resolver;
		WatchedListener<T> m_listener;
		std::optional<T> m_savedValue = std::nullopt;
	};

	template<typename ValueT, typename ResRetT>
	struct WatchedCapabilities<Watched<ValueT, ResRetT>> {
		static constexpr bool has_set_value = false;
	};

	// Specialization for ptr types
	template<typename T>
	class Watched<T, T*> : public WatchedBase {
	public:
		Resolver<T> GetResolver() {
			return m_resolver;
		}

		const std::optional<T>& GetSavedValue() const {
			return m_savedValue;
		}

		const std::optional<T>& GetNextValue() const {
			return m_nextValue;
		}

		void SetValue(const T& newValue, bool notify = false) {
			if (m_nextValue) {
				spdlog::warn("Watched::SetValue: Overwriting previously scheduled value change");
			}

			m_nextValue = newValue;
			m_nextValueNotify = notify;
		}

		bool SetValueNow(const T& newValue, bool notify = false) {
			T* valuePtr = m_resolver();
			if (valuePtr == nullptr) return false;
			*valuePtr = newValue;
			if(!notify) {
				m_savedValue = newValue;
			}
			return true;
		}

		void Update() override {
			T* valuePtr = m_resolver();
			bool present = (valuePtr != nullptr);

			if (m_nextValue.has_value()) {
				if (present) {
					*valuePtr = *m_nextValue;
					if(!m_nextValueNotify) {
						m_savedValue = *m_nextValue;
					}
				}
				else {
					spdlog::warn("Watched::Update: Tried to set value but pointer was null");
				}
				m_nextValue = std::nullopt;
			}

			bool changed = (present != m_savedValue.has_value()) ||
				(present && m_savedValue.has_value() && (*valuePtr != m_savedValue.value()));

			if (m_needsUpdate || changed) {
				std::optional<T> lastValue = m_savedValue;
				m_needsUpdate = false;
				m_savedValue = present ? std::optional<T>(*valuePtr) : std::nullopt;
				m_listener(lastValue, m_savedValue);
			}
		}

	private:
		friend class WatchManager;

		Watched(WatchedId id, Resolver<T*> resolver, WatchedListener<T> listener)
			: m_resolver(std::move(resolver)), m_listener(std::move(listener))
		{
			m_id = id;
		}

		Resolver<T*> m_resolver;
		WatchedListener<T> m_listener;
		std::optional<T> m_savedValue = std::nullopt;
		std::optional<T> m_nextValue = std::nullopt;
		bool m_nextValueNotify = false;
	};

	template<typename T>
	struct WatchedCapabilities<Watched<T, T*>> {
		static constexpr bool has_set_value = true;
	};

	// Specialization for const ptr types
	template<typename T>
	class Watched<T, const T*> : public WatchedBase {
	public:
		Resolver<const T*> GetResolver() {
			return m_resolver;
		}

		const std::optional<std::remove_const_t<T>>& GetSavedValue() const {
			return m_savedValue;
		}

		// No SetValue for const types (read-only)

		void Update() override {
			const T* valuePtr = m_resolver();
			bool present = (valuePtr != nullptr);

			bool changed = (present != m_savedValue.has_value()) ||
				(present && m_savedValue.has_value() && (*valuePtr != m_savedValue.value()));

			if (m_needsUpdate || changed) {
				std::optional<std::remove_const_t<T>> lastValue = m_savedValue;
				m_needsUpdate = false;
				m_savedValue = present ? std::optional<std::remove_const_t<T>>(*valuePtr) : std::nullopt;
				m_listener(lastValue, m_savedValue);
			}
		}

	private:
		friend class WatchManager;

		Watched(WatchedId id, Resolver<const T*> resolver, WatchedListener<T> listener)
			: m_resolver(std::move(resolver)), m_listener(std::move(listener))
		{
			m_id = id;
		}

		Resolver<const T*> m_resolver;
		WatchedListener<T> m_listener;
		std::optional<std::remove_const_t<T>> m_savedValue;
		bool m_needsUpdate = true;
	};

	template<typename T>
	struct WatchedCapabilities<Watched<T, const T*>> {
		static constexpr bool has_set_value = false;
	};

	// Specialization for tuple types where resolver returns a tuple of pointers
	template<typename... Values>
	class Watched<std::tuple<Values...>, std::tuple<Values*...>> : public WatchedBase {
	public:
		using Tuple = std::tuple<Values...>;
		using PtrTuple = std::tuple<Values*...>;

		Resolver<PtrTuple> GetResolver() {
			return m_resolver;
		}

		const std::optional<Tuple>& GetSavedValue() const {
			return m_savedValue;
		}

		const std::optional<Tuple>& GetNextValue() const {
			return m_nextValue;
		}

		void SetValue(const Tuple& newValue, bool notify = false) {
			if (m_nextValue) {
				spdlog::warn("Watched::SetValue: Overwriting previously scheduled value change");
			}

			m_nextValue = newValue;
			m_nextValueNotify = notify;
		}

		bool SetValueNow(const Tuple& newValue, bool notify = false) {
			PtrTuple ptrs = m_resolver();
			if (any_null(ptrs)) return false;
			assign_tuple(ptrs, newValue);
			if (!notify) {
				m_savedValue = newValue;
			}
			return true;
		}

		void Update() override {
			PtrTuple ptrs = m_resolver();
			bool present = !any_null(ptrs);

			if (m_nextValue.has_value()) {
				if (present) {
					assign_tuple(ptrs, *m_nextValue);
					if (!m_nextValueNotify) {
						m_savedValue = *m_nextValue;
					}
				}
				else {
					spdlog::warn("Watched::Update: Tried to set value but pointer was null");
				}
				m_nextValue = std::nullopt;
			}

			std::optional<Tuple> value = std::nullopt;
			if (present) value = deref_tuple(ptrs);

			if (m_needsUpdate || value != m_savedValue) {
				std::optional<Tuple> lastValue = m_savedValue;
				m_needsUpdate = false;
				m_savedValue = value;
				m_listener(lastValue, value);
			}
		}

	private:
		friend class WatchManager;

		Watched(WatchedId id, Resolver<PtrTuple> resolver, WatchedListener<Tuple> listener)
			: m_resolver(std::move(resolver)), m_listener(std::move(listener))
		{
			m_id = id;
		}

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

		Resolver<PtrTuple> m_resolver;
		WatchedListener<Tuple> m_listener;
		std::optional<Tuple> m_savedValue = std::nullopt;
		std::optional<Tuple> m_nextValue = std::nullopt;
		bool m_nextValueNotify = false;
	};

	template<typename... Values>
	struct WatchedCapabilities<Watched<std::tuple<Values...>, std::tuple<Values*...>>> {
		static constexpr bool has_set_value = true;
	};
}