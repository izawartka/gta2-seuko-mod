#pragma once
#include "../common.h"
#include "../component.h"

namespace UiModule {
	struct SelectableOptions {
		bool selected = false;
		Game::SCR_f markerScale = 0x6000;
		Game::SCR_f markerOffsetX = 0x60000;
		Game::SCR_f markerAdditionalOffsetY = 0x18000; /* doesn't affect component size */
		std::wstring markerText = L"*";
	};

	class Selectable : public Component {
	public:
		Selectable(Component* parent, SelectableOptions options = {});
		virtual void Draw() override;

		virtual void UpdateChildrenPos() override;

		void SetSelected(bool selected) { 
			m_options.selected = selected; 
		}
		bool IsSelected() const { return m_options.selected; }

	protected:
		SelectableOptions m_options;
	};
}