#pragma once
#include "../common.h"
#include "../component.h"

namespace UiModule {
	class Text : public Component {
	public:
		Text(Component* parent, const std::wstring& text, Game::SCR_f scale = 0x6000, short remap = -1);
		virtual void Draw() override;

		void SetText(const std::wstring& text);
		const std::wstring& GetText() const { return m_text; }
		void SetScale(Game::SCR_f scale);
		Game::SCR_f GetScale() const { return m_scale; }
		void SetRemap(short remap);
		short GetRemap() const { return m_remap; }

	protected:
		std::wstring m_text;
		Game::SCR_f m_scale;
		short m_remap;
	};
}