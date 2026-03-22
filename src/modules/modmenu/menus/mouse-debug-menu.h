#pragma once
#include "../common.h"
#include "../menu-base.h"

namespace ModMenuModule {
	/*
	Currently unused
	*/
	class MouseDebugMenu : public MenuBase, public Core::EventListenerSupport {
	public:
		MouseDebugMenu();
		virtual ~MouseDebugMenu();

	private:
		virtual bool Attach() override;
		virtual void OnShow() override;
		virtual void OnHide() override;

		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
		void OnPreUpdateUI(UiModule::PreUpdateUIEvent& event);
		void OnMouseMove(MouseModule::MouseMoveEvent& event);
		void OnMouseLockedMove(MouseModule::MouseLockedMoveEvent& event);
		void OnMouseButtonDown(MouseModule::MouseButtonDownEvent& event);
		void OnMouseButtonUp(MouseModule::MouseButtonUpEvent& event);
		void OnMouseEnter(MouseModule::MouseEnterEvent& event);
		void OnMouseLeave(MouseModule::MouseLeaveEvent& event);

		void UpdateState();
		static std::string ButtonNameToString(MouseModule::MouseButton button);

		UiModule::SelectController<bool>* m_subMouseMoveController = nullptr;
		UiModule::SelectController<bool>* m_subMouseLockedMoveController = nullptr;
		UiModule::SelectController<bool>* m_subMouseButtonDownController = nullptr;
		UiModule::SelectController<bool>* m_subMouseButtonUpController = nullptr;
		UiModule::SelectController<bool>* m_subMouseEnterController = nullptr;
		UiModule::SelectController<bool>* m_subMouseLeaveController = nullptr;
		UiModule::VarTextSelectController<bool, bool>* m_lockMouseController = nullptr;
		UiModule::VarTextSelectController<MouseModule::CursorVisibility, MouseModule::CursorVisibility>* m_cursorVisibilityController = nullptr;
		UiModule::Text* m_posXText = nullptr;
		UiModule::Text* m_posYText = nullptr;
		UiModule::Text* m_leftButtonText = nullptr;
		UiModule::Text* m_rightButtonText = nullptr;
		UiModule::Text* m_middleButtonText = nullptr;
		UiModule::Text* m_inWindowText = nullptr;
	};
}
