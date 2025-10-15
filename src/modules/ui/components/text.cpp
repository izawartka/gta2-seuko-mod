#include "text.h"

UiModule::Text::Text(Component* parent, const std::wstring& text, Game::SCR_f scale, short remap)
{
	m_text = text;
	m_scale = scale;
	m_rect.width = 0; /// TODO
	m_rect.height = scale * 20;
	m_remap = remap;

	if (parent) {
		parent->AddChild(this);
	}
}

void UiModule::Text::Draw()
{
	Game::PALETTE_BASE paletteBase = m_remap < 0
		? Game::PALETTE_BASE::PALETTE_BASE_SPRITE
		: Game::PALETTE_BASE::PALETTE_BASE_FONT_REMAP;

	short remap = m_remap < 0 ? 0 : m_remap;

	Game::Functions::DrawGTAText(
		(WCHAR*)m_text.c_str(),
		m_rect.x,
		m_rect.y,
		1,
		m_scale,
		&paletteBase,
		remap,
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

void UiModule::Text::SetRemap(short remap)
{
	m_remap = remap;
}

