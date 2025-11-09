#include "text.h"

UiModule::Text::Text(Component* parent, const std::wstring& text, Game::SCR_f scale, short remap)
{
	m_text = text;
	m_scale = scale;
	m_rect.width = GetTextWidth();
	m_rect.height = scale * 20;
	m_remap = remap;

	if (parent) {
		parent->AddChild(this);
	}
}

void UiModule::Text::Draw()
{
	short fontId = Game::Memory::GetDefaultFontId();
	if (fontId == 0) {
		spdlog::warn("UiModule::Text::Draw: Default font ID is 0, cannot draw text");
		return;
	}

	Game::PALETTE_BASE paletteBase = m_remap < 0
		? Game::PALETTE_BASE::PALETTE_BASE_SPRITE
		: Game::PALETTE_BASE::PALETTE_BASE_FONT_REMAP;

	short remap = m_remap < 0 ? 0 : m_remap;

	Game::Functions::DrawGTAText(
		m_text.c_str(),
		m_rect.x,
		m_rect.y,
		fontId,
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
	Game::SCR_f newWidth = GetTextWidth();
	UpdateSize(&newWidth, nullptr);
}

void UiModule::Text::SetScale(Game::SCR_f scale)
{
	m_scale = scale;
	Game::SCR_f newWidth = GetTextWidth();
	Game::SCR_f newHeight = scale * 20;
	UpdateSize(&newWidth, &newHeight);
}

void UiModule::Text::SetRemap(short remap)
{
	m_remap = remap;
}

Game::SCR_f UiModule::Text::GetTextWidth() const
{
	Game::Style_S3* styleS3 = Game::Memory::GetStyleS3();
	if(!styleS3) {
		spdlog::warn("UiModule::Text::GetTextWidth: Style_S3 is null, cannot get text width");
		return 0;
	}

	short fontId = Game::Memory::GetDefaultFontId();
	return Game::Functions::GetGTATextWidth(m_text.c_str(), fontId) * m_scale;
}

