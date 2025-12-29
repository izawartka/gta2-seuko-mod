#include "car-sprite.h"
#include "../utils/draw-sprite.h"

UiModule::CarSprite::CarSprite(Component* parent, const CarSpriteOptions& options)
{
	SetOptions(options);

	if (parent) {
		parent->AddChild(this);
	}
}

void UiModule::CarSprite::Draw()
{

	Game::Camera* mainCamera = Game::Memory::GetMainCamera();
	if (!mainCamera) {
		spdlog::warn("UiModule::CarSprite::Draw: Main camera is null, cannot get UI scale");
		return;
	}

	Game::SCR_f uiScale = mainCamera->uiScale;
	float scale = Game::Utils::ToFloat(Game::Utils::Multiply(m_options.scale, uiScale));
	float cx = Game::Utils::ToFloat(Game::Utils::Multiply(m_rect.x + m_spriteCenterX, uiScale));
	float cy = Game::Utils::ToFloat(Game::Utils::Multiply(m_rect.y + m_spriteCenterY, uiScale));

	Utils::DrawRotatedSprite(
		Game::SPRITE_TYPE_CAR,
		m_spriteId,
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

	if (m_roofSprite.spriteId != -1) {
		float roofCx = Game::Utils::ToFloat(Game::Utils::Multiply(m_rect.x + m_roofSprite.centerX, uiScale));
		float roofCy = Game::Utils::ToFloat(Game::Utils::Multiply(m_rect.y + m_roofSprite.centerY, uiScale));

		Utils::DrawRotatedSprite(
			Game::SPRITE_TYPE_CODE_OBJ,
			m_roofSprite.spriteId,
			Game::PALETTE_BASE_SPRITE,
			0,
			roofCx,
			roofCy,
			m_roofSprite.width,
			m_roofSprite.height,
			scale,
			m_options.rotation + (m_roofSprite.isReversed ? static_cast<float>(M_PI) : 0.0f),
			m_options.alpha
		);
	}
}

void UiModule::CarSprite::SetOptions(const CarSpriteOptions& options)
{
	m_options = options;
	UpdateCarModelData();
	UpdateRoofSpriteData();
	RecalculateSize();
}

void UiModule::CarSprite::UpdateCarModelData()
{
	Game::CAR_MODEL4 model = m_options.carModel;
	if (m_options.fixHotdogD3 && model == Game::CAR_MODEL4_HOTDOG_D3) {
		model = Game::CAR_MODEL4_HOTDOG_D2;
	}

	Game::Style_S3* styleS3 = Game::Memory::GetStyleS3();
	Game::CarInfo* carInfo = styleS3->allCarsInfo->cars[model];
	if (!carInfo) {
		spdlog::error("UiModule::CarSprite::UpdateCarModelData: Could not find car info for the model #{}", static_cast<uint32_t>(model));
		return;
	}
	m_spriteId = static_cast<uint16_t>(carInfo->sprite);
	m_carWidth = carInfo->w;
	m_carHeight = carInfo->h;

	Game::uint spriteIndex = Game::Functions::GetSpriteIndex(styleS3, 0, Game::SPRITE_TYPE_CAR, m_spriteId);
	Game::SpriteEntry* spriteEntry = Game::Functions::GetSpriteEntry(styleS3, 0, spriteIndex);
	m_spriteWidth = spriteEntry->width;
	m_spriteHeight = spriteEntry->height;
}

void UiModule::CarSprite::UpdateRoofSpriteData()
{
	m_roofSprite = {};
	if (!m_options.autoRoof) return;

	m_roofSprite.spriteId = Game::Utils::GetCarRoofSpriteForModel(m_options.carModel);
	if (m_roofSprite.spriteId == -1) return;

	int* roofOffsetY = Game::Utils::GetCarRoofOffsetY(m_roofSprite.spriteId);
	if (!roofOffsetY) return;

	Game::Style_S3* styleS3 = Game::Memory::GetStyleS3();
	Game::uint spriteIndex = Game::Functions::GetSpriteIndex(styleS3, 0, Game::SPRITE_TYPE_CODE_OBJ, m_roofSprite.spriteId);
	Game::SpriteEntry* spriteEntry = Game::Functions::GetSpriteEntry(styleS3, 0, spriteIndex);
	m_roofSprite.width = spriteEntry->width;
	m_roofSprite.height = spriteEntry->height;
	m_roofSprite.offsetX = 0;
	m_roofSprite.offsetY = Game::Utils::Multiply(static_cast<Game::SCR_f>(*roofOffsetY), m_options.scale) * 64;
	m_roofSprite.isReversed = m_roofSprite.spriteId == 278;
}

void UiModule::CarSprite::RecalculateSize()
{
	Game::SCR_f width = m_carWidth * m_options.scale;
	Game::SCR_f height = m_carHeight * m_options.scale;
	Game::SCR_f spriteCx = width / 2;
	Game::SCR_f spriteCy = height / 2;

	Game::SCR_f cosRot = Game::Utils::FromFloat(cos(m_options.rotation));
	Game::SCR_f sinRot = Game::Utils::FromFloat(sin(m_options.rotation));
	Game::SCR_f absCosRot = abs(cosRot);
	Game::SCR_f absSinRot = abs(sinRot);

	if (m_roofSprite.spriteId == -1) {
		Game::SCR_f rectWidth = Game::Utils::Multiply(absCosRot, width) + Game::Utils::Multiply(absSinRot, height);
		Game::SCR_f rectHeight = Game::Utils::Multiply(absSinRot, width) + Game::Utils::Multiply(absCosRot, height);
		m_spriteCenterX = rectWidth / 2;
		m_spriteCenterY = rectHeight / 2;
		UpdateSize(&rectWidth, &rectHeight);
		return;
	}

	Game::SCR_f roofWidth = m_roofSprite.width * m_options.scale;
	Game::SCR_f roofMinX = spriteCx + m_roofSprite.offsetX - roofWidth / 2;
	Game::SCR_f roofMaxX = spriteCx + m_roofSprite.offsetX + roofWidth / 2;
	if (roofMinX < 0) {
		width += -roofMinX;
		spriteCx += -roofMinX;
		roofMaxX += -roofMinX;
	}
	if (roofMaxX > width) {
		width = roofMaxX;
	}

	Game::SCR_f roofHeight = m_roofSprite.height * m_options.scale;
	Game::SCR_f roofMinY = spriteCy + m_roofSprite.offsetY - roofHeight / 2; 
	Game::SCR_f roofMaxY = spriteCy + m_roofSprite.offsetY + roofHeight / 2;
	if (roofMinY < 0) {
		height += -roofMinY;
		spriteCy += -roofMinY;
		roofMaxY += -roofMinY;
	}
	if (roofMaxY > height) {
		height = roofMaxY;
	}

	Game::SCR_f rectWidth = Game::Utils::Multiply(absCosRot, width) + Game::Utils::Multiply(absSinRot, height);
	Game::SCR_f rectHeight = Game::Utils::Multiply(absSinRot, width) + Game::Utils::Multiply(absCosRot, height);
	Game::SCR_f rectCx = rectWidth / 2;
	Game::SCR_f rectCy = rectHeight / 2;

	Game::SCR_f spriteCxOffset = spriteCx - width / 2;
	Game::SCR_f spriteCyOffset = spriteCy - height / 2;

	m_spriteCenterX = rectCx + Game::Utils::Multiply(cosRot, spriteCxOffset) - Game::Utils::Multiply(sinRot, spriteCyOffset);
	m_spriteCenterY = rectCy + Game::Utils::Multiply(sinRot, spriteCxOffset) + Game::Utils::Multiply(cosRot, spriteCyOffset);

	Game::SCR_f roofCx = spriteCx + m_roofSprite.offsetX;
	Game::SCR_f roofCy = spriteCy + m_roofSprite.offsetY;
	Game::SCR_f roofCxOffset = roofCx - width / 2;
	Game::SCR_f roofCyOffset = roofCy - height / 2;

	m_roofSprite.centerX = rectCx + Game::Utils::Multiply(cosRot, roofCxOffset) - Game::Utils::Multiply(sinRot, roofCyOffset);
	m_roofSprite.centerY = rectCy + Game::Utils::Multiply(sinRot, roofCxOffset) + Game::Utils::Multiply(cosRot, roofCyOffset);

	UpdateSize(&rectWidth, &rectHeight);
}
