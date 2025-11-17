#pragma once
#include "common.h"

namespace ModMenuModule {
	struct QuickActionData {};

	class QuickActionBase {
	public:
		using DataT = QuickActionData;

		virtual ~QuickActionBase() = default;
		virtual void Execute() = 0;
		virtual const std::wstring& GetLabel() const = 0;

	protected:
		QuickActionBase() = default;
		QuickActionBase(const QuickActionBase&) = delete;
		QuickActionBase& operator=(const QuickActionBase&) = delete;
	};
}
