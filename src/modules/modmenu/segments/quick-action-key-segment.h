#pragma once
#include "../common.h"
#include "../segment-base.h"
#include "../quick-action-manager.h"

namespace ModMenuModule {
	struct QuickActionKeySegmentData {
		KeyBindingModule::Key key;
	};

	class QuickActionKeySegment : public Segment<QuickActionKeySegmentData>, public Core::EventListenerSupport {
	public:
		QuickActionKeySegment(QuickActionId actionId = -1);
		virtual ~QuickActionKeySegment();

		virtual std::optional<QuickActionKeySegmentData> GetSegmentData() const override;
		virtual bool SetSegmentData(const QuickActionKeySegmentData& data) override;

		bool IsKeyTaken() const;

	private:
		virtual bool Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent) override;
		virtual void Detach() override;

		virtual void OnShow() override;
		virtual void OnHide() override;

		void OnKeyBindUpdate(KeyBindingModule::KeyBindUpdateEvent& event);
		void OnKeyEditStateChange(bool editing);

		void SetShowKeyTakenText(bool show);
		void UpdateKeyTakenText();

		QuickActionId m_actionId = -1;
		UiModule::KeyChangeController* m_keyController = nullptr;
		UiModule::VertCont* m_keyTakenContainer = nullptr;
		UiModule::Text* m_keyTakenText = nullptr;
	};
}
