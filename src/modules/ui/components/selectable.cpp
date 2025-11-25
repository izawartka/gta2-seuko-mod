#include "selectable.h"

UiModule::Selectable::Selectable(Component* parent, const SelectableOptions& options)
{
	m_options = options;
	m_rect.width = options.markerOffsetX;
	m_rect.height = options.markerScale * 20;

	if (parent) {
		parent->AddChild(this);
	}
}

void UiModule::Selectable::Draw()
{
	if(!m_options.selected) {
		return;
	}

	Game::Camera* mainCamera = Game::Memory::GetMainCamera();
	if (!mainCamera) {
		spdlog::warn("UiModule::Selectable::Draw: Main camera is null, cannot get UI scale");
		return;
	}
	Game::SCR_f uiScale = mainCamera->uiScale;

	Game::SCR_f x = Game::Utils::Multiply(m_rect.x, uiScale);
	Game::SCR_f additionalYOffset = Game::Utils::Multiply(m_options.markerAdditionalOffsetY, m_rect.height);
	Game::SCR_f y = Game::Utils::Multiply(m_rect.y + additionalYOffset, uiScale);
	Game::PALETTE_BASE paletteBase = Game::PALETTE_BASE::PALETTE_BASE_SPRITE;
	Game::Functions::DrawGTAText(
		(WCHAR*)m_options.markerText.c_str(),
		x,
		y,
		1,
		Game::Utils::Multiply(m_options.markerScale, uiScale),
		&paletteBase,
		0,
		Game::SPRITE_INVISIBILITY_VISIBLE,
		0
	);
}

void UiModule::Selectable::UpdateChildrenPos()
{
	Game::SCR_f maxWidth = m_options.markerOffsetX;
	Game::SCR_f maxHeight = 0;
	for (auto child : m_children) {
		child->SetPosition(m_rect.x + m_options.markerOffsetX, m_rect.y);
		const Rect& childRect = child->GetRect();
		maxWidth = std::max(maxWidth, m_options.markerOffsetX + childRect.width);
		maxHeight = std::max(maxHeight, childRect.height);

	}

	UpdateSize(&maxWidth, &maxHeight);
}
