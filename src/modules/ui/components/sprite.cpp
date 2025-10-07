#include "sprite.h"

UiModule::Sprite::Sprite(Component* parent, SpriteOptions options)
{
	m_spriteType = options.spriteType;
	m_spriteId = options.spriteId;
	m_palette = options.palette;
	m_remap = options.remap;
	m_scale = options.scale;
	m_rotation = options.rotation;

	RecalculateSize();

	if (parent) {
		parent->AddChild(this);
	}
}

void UiModule::Sprite::Draw()
{
	Game::S4* s4 = Game::Memory::GetS4();
	void* sprite = Game::Functions::GetSpriteTexture(s4, 0, m_spriteType, m_spriteId, m_palette, m_remap);

	float scale = Game::Utils::ToFloat(m_scale);
	float halfWidth = (float)m_spriteWidth * scale / 2.0f;
	float halfHeight = (float)m_spriteHeight * scale / 2.0f;
	float cx = Game::Utils::ToFloat(m_rect.x + m_rect.width / 2);
	float cy = Game::Utils::ToFloat(m_rect.y + m_rect.height / 2);
	float cosRot = cos(m_rotation);
	float sinRot = sin(m_rotation);

	Game::GTAVertex vertices[4]{};
	vertices[0].x = cx - halfWidth * cosRot + halfHeight * sinRot;
	vertices[0].y = cy - halfWidth * sinRot - halfHeight * cosRot;
	vertices[0].z = 0.14f;
	vertices[0].z2 = 0.14f;
	vertices[0].field_0x10 = 0xa0000000;
	vertices[0].u = 0.3f;
	vertices[0].v = 0.3f;

	vertices[1].x = cx + halfWidth * cosRot + halfHeight * sinRot;
	vertices[1].y = cy + halfWidth * sinRot - halfHeight * cosRot;
	vertices[1].z = 0.14f;
	vertices[1].z2 = 0.14f;
	vertices[1].field_0x10 = 0xa0000000;
	vertices[1].u = (float)m_spriteWidth - 0.3f;
	vertices[1].v = 0.3f;

	vertices[2].x = cx + halfWidth * cosRot - halfHeight * sinRot;
	vertices[2].y = cy + halfWidth * sinRot + halfHeight * cosRot;
	vertices[2].z = 0.14f;
	vertices[2].z2 = 0.14f;
	vertices[2].field_0x10 = 0xa0000000;
	vertices[2].u = (float)m_spriteWidth - 0.3f;
	vertices[2].v = (float)m_spriteHeight - 0.3f;

	vertices[3].x = cx - halfWidth * cosRot - halfHeight * sinRot;
	vertices[3].y = cy - halfWidth * sinRot + halfHeight * cosRot;
	vertices[3].z = 0.14f;
	vertices[3].z2 = 0.14f;
	vertices[3].field_0x10 = 0xa0000000;
	vertices[3].u = 0.3f;
	vertices[3].v = (float)m_spriteHeight - 0.3f;

	Game::Functions::DrawQuad(
		0x80,
		(DWORD)sprite,
		vertices,
		0xff
	);
}

void UiModule::Sprite::SetSprite(Game::SPRITE_TYPE spriteType, uint16_t spriteId)
{
	m_spriteType = spriteType;
	m_spriteId = spriteId;
	RecalculateSize();
}

void UiModule::Sprite::SetPalette(Game::PALETTE_BASE palette)
{
	m_palette = palette;
}

void UiModule::Sprite::SetRemap(uint16_t remap)
{
	m_remap = remap;
}

void UiModule::Sprite::SetScale(Game::SCR_f scale)
{
	m_scale = scale;
	RecalculateSize();
}

void UiModule::Sprite::SetRotation(float rotation)
{
	m_rotation = rotation;
	RecalculateSize();
}

void UiModule::Sprite::RecalculateSize()
{
	Game::Style_S3* styleS3 = Game::Memory::GetStyleS3();
	Game::uint spriteIndex = Game::Functions::GetSpriteIndex(styleS3, 0, m_spriteType, m_spriteId);
	Game::SpriteEntry* spriteEntry = Game::Functions::GetSpriteEntry(styleS3, 0, spriteIndex);
	m_spriteWidth = spriteEntry->width;
	m_spriteHeight = spriteEntry->height;

	float scale = Game::Utils::ToFloat(m_scale);
	float baseWidth = m_spriteWidth * scale;
	float baseHeight = m_spriteHeight * scale;

	float absCosRot = abs(cos(m_rotation));
	float absSinRot = abs(sin(m_rotation));
	float width = absCosRot * baseWidth + absSinRot * baseHeight;
	float height = absSinRot * baseWidth + absCosRot * baseHeight;

	Game::SCR_f rectWidth = Game::Utils::FromFloat(width);
	Game::SCR_f rectHeight = Game::Utils::FromFloat(height);
	UpdateSize(&rectWidth, &rectHeight);
}
