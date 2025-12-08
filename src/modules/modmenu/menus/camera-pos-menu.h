#pragma once
#include "../common.h"
#include "../menu-base.h"
#include "../cheat-support.h"
#include "../events/cheat-state.h"
#include "../segments/camera-pos-cheat-coord-segment.h"

namespace ModMenuModule {
	class CameraPosMenu : public MenuBase, public Core::EventListenerSupport, public CheatSupport {
	public:
		CameraPosMenu();
		virtual ~CameraPosMenu();

		virtual bool Attach() override;
		virtual void Detach() override;
		virtual void OnShow() override;
		virtual void OnHide() override;

	private:
		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;

		void AttachCheatMenuItems();
		void DetachCheatMenuItems();
		void OnCheatStateChange(CheatStateEvent& event);
		void UpdateCheatStates();

		UiModule::VertCont* m_cheatItemsCont = nullptr;
		UiModule::VertCont* m_cheatItemsInnerCont = nullptr;
		UiModule::SelectController<bool>* m_cameraPosCheatController = nullptr;
		UiModule::MenuItemGroupId m_cheatItemsGroupId = -1;
		bool m_cheatItemsAttached = false;

		CameraPosCheatCoordSegment m_xPosSegment{ L"X: ", 0 };
		CameraPosCheatCoordSegment m_yPosSegment{ L"Y: ", 1 };
		CameraPosCheatCoordSegment m_zPosSegment{ L"Z: ", 2 };
		CameraPosCheatCoordSegment m_zoomSegment{ L"Zoom: ", 3 };
	};
}
