#include "text.h"

UiModule::Text::Text(Component* parent, const std::wstring& text, Game::SCR_f scale)
{
	m_text = text;
	m_scale = scale;
	m_rect.width = 0; /// TODO
	m_rect.height = scale * 20;

	if (parent) {
		parent->AddChild(this);
	}
}

void UiModule::Text::Draw()
{
	Game::S4_ENUM1 s4enum2 = (Game::S4_ENUM1)2;
	Game::Functions::DrawGTAText(
		(WCHAR*)m_text.c_str(),
		m_rect.x,
		m_rect.y,
		1,
		m_scale,
		&s4enum2,
		1,
		Game::SPRITE_INVISIBILITY_VISIBLE,
		1
	);
}

void UiModule::Text::SetText(const std::wstring& text)
{
	m_text = text;
	/// TODO: update width
}

void UiModule::Text::SetScale(Game::SCR_f scale)
{
	m_scale = scale;
	Game::SCR_f newHeight = scale * 20;
	UpdateSize(nullptr, &newHeight);
}

