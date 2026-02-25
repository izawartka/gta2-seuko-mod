#pragma once
#include "../common.h"

namespace ModMenuModule {
	template<typename, typename = void>
	struct has_options_t : std::false_type {};

	template<typename T>
	struct has_options_t<T, std::void_t<typename T::OptionsT>> : std::true_type {};
	template<typename CheatT>
	class CheatOptionsUpdateEvent : public Core::EventBase {
		static_assert(has_options_t<CheatT>::value, "CheatT must define `OptionsT` (e.g. `using OptionsT = ...;`)");
		using OptionsT = typename CheatT::OptionsT;
	public:
		CheatOptionsUpdateEvent(const OptionsT& oldOptions, const OptionsT& newOptions) : m_oldOptions(oldOptions), m_newOptions(newOptions) {}

		const OptionsT& GetOldOptions() const { return m_oldOptions; }
		const OptionsT& GetNewOptions() const { return m_newOptions; }

	private:
		OptionsT m_oldOptions;
		OptionsT m_newOptions;
	};
}
