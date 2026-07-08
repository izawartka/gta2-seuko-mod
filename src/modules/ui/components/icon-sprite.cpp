#include "icon-sprite.h"
#include "../utils/draw-sprite.h"

UiModule::IconSprite::IconSprite(Component* parent, const IconSpriteOptions& options)
{
	SetOptions(options);

	if (parent) {
		parent->AddChild(this);
	}
}

void UiModule::IconSprite::Draw()
{
	Game::Camera* mainCamera = Game::Memory::GetMainCamera();
	if (!mainCamera) {
		spdlog::warn("UiModule::IconSprite::Draw: Main camera is null, cannot get UI scale");
		return;
	}

	Game::SCR_f uiScale = mainCamera->uiScale;
	float fUiScale = Game::Utils::ToFloat(uiScale);
	float fRectWidth = Game::Utils::ToFloat(m_rect.width);
	float fRectHeight = Game::Utils::ToFloat(m_rect.height);
	float fSpriteWidth = static_cast<float>(m_spriteWidth);
	float fSpriteHeight = static_cast<float>(m_spriteHeight);

	float widthRatio = fSpriteWidth / fRectWidth;
	float heightRatio = fSpriteHeight / fRectHeight;
	float aspectRatio = fSpriteWidth / fSpriteHeight;

	float karWidth, karHeight;
	if (widthRatio < heightRatio) {
		karWidth = fRectHeight * aspectRatio;
		karHeight = fRectHeight;
	} 
	else
	{
		karWidth = fRectWidth;
		karHeight = fRectWidth / aspectRatio;
	}

	float cx = Game::Utils::ToFloat(Game::Utils::Multiply(m_rect.x + m_rect.width / 2, uiScale));
	float cy = Game::Utils::ToFloat(Game::Utils::Multiply(m_rect.y + m_rect.height / 2, uiScale));

	float halfWidth = m_options.keepAspectRatio ? 
		(karWidth * fUiScale) / 2.0f :
		(fRectWidth * fUiScale) / 2.0f;

	float halfHeight = m_options.keepAspectRatio ? 
		(karHeight * fUiScale) / 2.0f :
		(fRectHeight * fUiScale) / 2.0f;

	float x1 = cx - halfWidth;
	float y1 = cy - halfHeight;
	float x2 = cx + halfWidth;
	float y2 = cy + halfHeight;

	Utils::DrawSprite(
		m_options.spriteType,
		m_options.spriteId,
		m_options.palette,
		m_options.remap,
		x1,
		y1,
		x2,
		y2,
		m_spriteWidth,
		m_spriteHeight,
		m_options.alpha
	);
}

void UiModule::IconSprite::SetOptions(const IconSpriteOptions& options)
{
	m_options = options;
	RecalculateSize();
}

void UiModule::IconSprite::RecalculateSize()
{
	Game::Style_S3* styleS3 = Game::Memory::GetStyleS3();
	Game::uint spriteIndex = Game::Functions::GetSpriteIndex(styleS3, 0, m_options.spriteType, m_options.spriteId);
	Game::SpriteEntry* spriteEntry = Game::Functions::GetSpriteEntry(styleS3, 0, spriteIndex);
	m_spriteWidth = spriteEntry->width;
	m_spriteHeight = spriteEntry->height;

	Game::SCR_f rectWidth = m_options.width;
	Game::SCR_f rectHeight = m_options.height;
	UpdateSize(&rectWidth, &rectHeight);
}
