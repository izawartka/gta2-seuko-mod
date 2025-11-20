#pragma once
#include "common.h"

namespace ModMenuModule {
	class MenuBase;

	class SegmentBase {
	public:
		virtual ~SegmentBase() = default;
		SegmentBase() = default;

		virtual bool Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent);
		virtual void Detach();

		virtual void OnShow() {}
		virtual void OnHide() {}

	protected:
		friend class MenuBase;

		virtual void CreateSegment(ModMenuModule::MenuBase* menu, UiModule::Component* parent) final;
		virtual void DestroySegment() final;

		ModMenuModule::MenuBase* m_menu = nullptr;
		UiModule::Component* m_parent = nullptr;
		UiModule::VertCont* m_vertCont = nullptr;
		UiModule::MenuController* m_menuController = nullptr;
	};

	template<typename DataT>
	class Segment : public SegmentBase {
		public:
		virtual std::optional<DataT> GetSegmentData() const = 0;
		virtual bool SetSegmentData(const DataT& data) = 0;
	};
}
