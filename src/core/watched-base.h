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

	template<typename WatchedT>
	static constexpr bool WatchedHasGetPointer_v = WatchedCapabilities<WatchedT>::has_get_pointer;

	// Specialization for value types
	template<typename T>
	class Watched<T, T> : public WatchedBase {
	public:
		std::optional<T> GetValue() {
			return m_resolver();
		}

		void Update() override {
			T value = m_resolver();

			if (m_needsUpdate || !m_lastValue.has_value() || value != m_lastValue.value()) {
				std::optional<T> lastValue = m_lastValue;
				m_needsUpdate = false;
				m_lastValue = value;
				m_listener(lastValue, value);
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
		std::optional<T> m_lastValue;
	};

	template<typename ValueT, typename ResRetT>
	struct WatchedCapabilities<Watched<ValueT, ResRetT>> {
		static constexpr bool has_set_value = false;
		static constexpr bool has_get_pointer = false;
	};

	// Specialization for ptr types
	template<typename T>
	class Watched<T, T*> : public WatchedBase {
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

		bool SetValueNoNotify(T value) {
			if (SetValue(value)) {
				m_lastValue = value;
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

	private:
		friend class WatchManager;

		Watched(WatchedId id, Resolver<T*> resolver, WatchedListener<T> listener)
			: m_resolver(std::move(resolver)), m_listener(std::move(listener))
		{
			m_id = id;
		}

		Resolver<T*> m_resolver;
		WatchedListener<T> m_listener;
		std::optional<T> m_lastValue;
	};

	template<typename T>
	struct WatchedCapabilities<Watched<T, T*>> {
		static constexpr bool has_set_value = true;
		static constexpr bool has_get_pointer = true;
	};

	// Specialization for const ptr types
	template<typename T>
	class Watched<T, const T*> : public WatchedBase {
	public:
		const T* GetPointer() {
			return m_resolver();
		}

		std::optional<T> GetValue() {
			const T* p = m_resolver();
			if (p) return *p;
			return std::nullopt;
		}

		// No SetValue for const types (read-only)

		void Update() override {
			const T* p = m_resolver();
			bool present = (p != nullptr);
			std::optional<std::remove_const_t<T>> value = std::nullopt;
			if (present) value = *p;

			if (m_needsUpdate || value != m_lastValue) {
				std::optional<std::remove_const_t<T>> lastValue = m_lastValue;
				m_needsUpdate = false;
				m_lastValue = value;
				m_listener(lastValue, value);
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
		std::optional<std::remove_const_t<T>> m_lastValue;
		bool m_needsUpdate = true;
	};

	template<typename T>
	struct WatchedCapabilities<Watched<T, const T*>> {
		static constexpr bool has_set_value = false;
		static constexpr bool has_get_pointer = true;
	};

	// Specialization for tuple types where resolver returns a tuple of pointers
	template<typename... Values>
	class Watched<std::tuple<Values...>, std::tuple<Values*...>> : public WatchedBase {
	public:
		using Tuple = std::tuple<Values...>;
		using PtrTuple = std::tuple<Values*...>;

		// No GetPointer for tuple

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

		bool SetValueNoNotify(Tuple value) {
			if (SetValue(value)) {
				m_lastValue = value;
				return true;
			}
			return false;
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
		std::optional<Tuple> m_lastValue;
		bool m_needsUpdate = true;
	};

	template<typename... Values>
	struct WatchedCapabilities<Watched<std::tuple<Values...>, std::tuple<Values*...>>> {
		static constexpr bool has_set_value = true;
		static constexpr bool has_get_pointer = false;
	};
}