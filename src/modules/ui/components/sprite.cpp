#include "sprite.h"
#include "../utils/draw-sprite.h"

UiModule::Sprite::Sprite(Component* parent, const SpriteOptions& options)
{
	SetOptions(options);

	if (parent) {
		parent->AddChild(this);
	}
}

void UiModule::Sprite::Draw()
{
	Game::Camera* mainCamera = Game::Memory::GetMainCamera();
	if (!mainCamera) {
		spdlog::warn("UiModule::Sprite::Draw: Main camera is null, cannot get UI scale");
		return;
	}

	Game::SCR_f uiScale = mainCamera->uiScale;
	float scale = Game::Utils::ToFloat(Game::Utils::Multiply(m_options.scale, uiScale));
	float cx = Game::Utils::ToFloat(Game::Utils::Multiply(m_rect.x + m_rect.width / 2, uiScale));
	float cy = Game::Utils::ToFloat(Game::Utils::Multiply(m_rect.y + m_rect.height / 2, uiScale));

	Utils::DrawRotatedSprite(
		m_options.spriteType,
		m_options.spriteId,
		m_options.palette,
		m_options.remap,
		cx,
		cy,
		m_spriteWidth,
		m_spriteHeight,
		scale,
		m_options.rotation,
		m_options.alpha
	);
}

void UiModule::Sprite::SetOptions(const SpriteOptions& options)
{
	m_options = options;
	RecalculateSize();
}

void UiModule::Sprite::RecalculateSize()
{
	Game::Style_S3* styleS3 = Game::Memory::GetStyleS3();
	Game::uint spriteIndex = Game::Functions::GetSpriteIndex(styleS3, 0, m_options.spriteType, m_options.spriteId);
	Game::SpriteEntry* spriteEntry = Game::Functions::GetSpriteEntry(styleS3, 0, spriteIndex);
	m_spriteWidth = spriteEntry->width;
	m_spriteHeight = spriteEntry->height;

	float scale = Game::Utils::ToFloat(m_options.scale);
	float baseWidth = m_spriteWidth * scale;
	float baseHeight = m_spriteHeight * scale;

	float absCosRot = abs(cos(m_options.rotation));
	float absSinRot = abs(sin(m_options.rotation));
	float width = absCosRot * baseWidth + absSinRot * baseHeight;
	float height = absSinRot * baseWidth + absCosRot * baseHeight;

	Game::SCR_f rectWidth = Game::Utils::FromFloat(width);
	Game::SCR_f rectHeight = Game::Utils::FromFloat(height);
	UpdateSize(&rectWidth, &rectHeight);
}
