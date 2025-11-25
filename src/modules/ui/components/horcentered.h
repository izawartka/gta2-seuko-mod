#pragma once
#include "../common.h"
#include "../component.h"

namespace UiModule {
	class HorCentered : public Component {
	public:
		HorCentered(Component* parent);
		virtual void UpdateChildrenPos() override;
	};
}
