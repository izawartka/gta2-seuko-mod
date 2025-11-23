#pragma once
#include "../common.h"
#include "../component.h"

namespace UiModule {
	struct BackgroundOptions {
		uint8_t alpha = 128;
	};

	class Background : public Component {
	public:
		Background(Component* parent, const BackgroundOptions& options);
		virtual void Draw() override;
		virtual void UpdateChildrenPos() override;

		void SetOptions(const BackgroundOptions& options) { m_options = options; }
		const BackgroundOptions& GetOptions() const { return m_options; }

	protected:
		BackgroundOptions m_options;
	};
}
