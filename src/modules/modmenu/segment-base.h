#pragma once
#include "common.h"

namespace ModMenuModule {
	class MenuBase;

	class SegmentBase {
	public:
		virtual ~SegmentBase();
		SegmentBase() = default;

		virtual bool IsAttached() const final { return m_attached; }
		virtual bool IsVisible() const final { return m_visible; }

	protected:
		friend class SegmentSupport;
		virtual void SetAttached(bool attached, ModMenuModule::MenuBase* menu = nullptr, UiModule::Component* parent = nullptr) final;
		virtual void SetVisible(bool visible) final;

		virtual bool Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent);
		virtual void Detach();

		virtual void OnShow() {}
		virtual void OnHide() {}

		virtual void CreateSegment(ModMenuModule::MenuBase* menu, UiModule::Component* parent) final;
		virtual void DestroySegment() final;

		bool m_attached = false;
		bool m_visible = false;
		ModMenuModule::MenuBase* m_menu = nullptr;
		UiModule::Component* m_parent = nullptr;
		UiModule::VertCont* m_vertCont = nullptr;
		UiModule::MenuController* m_menuController = nullptr;
		UiModule::MenuItemGroupId m_menuGroupId = -1;
	};

	template<typename DataT>
	class Segment : public SegmentBase {
	public:
		virtual std::optional<DataT> GetSegmentData() const = 0;
		virtual bool SetSegmentData(const DataT& data) = 0;
	};
}
