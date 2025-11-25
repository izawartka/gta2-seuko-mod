#pragma once
#include "../common.h"
#include "../component.h"

namespace UiModule {
	struct SelectableOptions {
		bool selected = false;
		Game::SCR_f markerScale = Game::Utils::FromFloat(1.0f);
		Game::SCR_f markerOffsetX = Game::Utils::FromFloat(15.0f);
		Game::SCR_f markerAdditionalOffsetY = Game::Utils::FromFloat(0.2f); /* relative to marker size; doesn't affect component size */
		std::wstring markerText = L"*";
	};

	class Selectable : public Component {
	public:
		Selectable(Component* parent, const SelectableOptions& options = {});
		virtual void Draw() override;

		virtual void UpdateChildrenPos() override;

		void SetSelected(bool selected) { m_options.selected = selected; }
		bool IsSelected() const { return m_options.selected; }

	protected:
		SelectableOptions m_options;
	};
}