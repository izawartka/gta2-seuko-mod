#pragma once
#include "../common.h"
#include "../component.h"

namespace UiModule {
	struct BackgroundOptions {
		uint8_t alpha = 128;
	};

	class Background : public Component {
	public:
		Background(Component* parent, BackgroundOptions options);
		virtual void Draw() override;
		virtual void UpdateChildrenPos() override;

		void SetOptions(BackgroundOptions options) { m_options = options; }
		BackgroundOptions GetOptions() const { return m_options; }

	protected:
		BackgroundOptions m_options;
	};
}
