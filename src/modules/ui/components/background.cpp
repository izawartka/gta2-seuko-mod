#include "background.h"
#include "../utils/draw-sprite.h"

UiModule::Background::Background(Component* parent, const BackgroundOptions& options)
{
	SetOptions(options);

	if (parent) {
		parent->AddChild(this);
	}
}

void UiModule::Background::Draw()
{
	Game::Camera* mainCamera = Game::Memory::GetMainCamera();
	if (!mainCamera) {
		spdlog::warn("UiModule::Background::Draw: Main camera is null, cannot get UI scale");
		return;
	}
	Game::SCR_f uiScale = mainCamera->uiScale;

	float x1 = Game::Utils::ToFloat(Game::Utils::Multiply(m_rect.x, uiScale));
	float y1 = Game::Utils::ToFloat(Game::Utils::Multiply(m_rect.y, uiScale));
	float x2 = Game::Utils::ToFloat(Game::Utils::Multiply(m_rect.x + m_rect.width, uiScale));
	float y2 = Game::Utils::ToFloat(Game::Utils::Multiply(m_rect.y + m_rect.height, uiScale));

	Utils::DrawSprite(
		Game::SPRITE_TYPE_CODE_OBJ,
		309,
		Game::PALETTE_BASE_SPRITE,
		0,
		x1,
		y1,
		x2,
		y2,
		1,
		1,
		m_options.alpha
	);
}

void UiModule::Background::UpdateChildrenPos() {
	Game::SCR_f maxHeight = 0;
	Game::SCR_f maxWidth = 0;
	for (auto child : m_children) {
		child->SetPosition(m_rect.x, m_rect.y);
		const Rect& childRect = child->GetRect();
		maxWidth = std::max(maxWidth, childRect.width);
		maxHeight = std::max(maxHeight, childRect.height);
	}

	UpdateSize(&maxWidth, &maxHeight);
}
