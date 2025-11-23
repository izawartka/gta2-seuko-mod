#include "sprite.h"

UiModule::Sprite::Sprite(Component* parent, const SpriteOptions& options)
{
	SetOptions(options);

	if (parent) {
		parent->AddChild(this);
	}
}

void UiModule::Sprite::Draw()
{
	Game::S4* s4 = Game::Memory::GetS4();
	void* sprite = Game::Functions::GetSpriteTexture(
		s4, 
		0, 
		m_options.spriteType, 
		m_options.spriteId, 
		m_options.palette, 
		m_options.remap
	);

	float scale = Game::Utils::ToFloat(m_options.scale);
	float halfWidth = (float)m_spriteWidth * scale / 2.0f;
	float halfHeight = (float)m_spriteHeight * scale / 2.0f;
	float cx = Game::Utils::ToFloat(m_rect.x + m_rect.width / 2);
	float cy = Game::Utils::ToFloat(m_rect.y + m_rect.height / 2);
	float cosRot = cos(m_options.rotation);
	float sinRot = sin(m_options.rotation);

	uint32_t vertexFlags = 0x00ffffff | (static_cast<uint32_t>(m_options.alpha) << 24);

	Game::GTAVertex vertices[4]{};
	vertices[0].x = cx - halfWidth * cosRot + halfHeight * sinRot;
	vertices[0].y = cy - halfWidth * sinRot - halfHeight * cosRot;
	vertices[0].z = 0.14f;
	vertices[0].z2 = 0.14f;
	vertices[0].flags = vertexFlags;
	vertices[0].u = 0.3f;
	vertices[0].v = 0.3f;

	vertices[1].x = cx + halfWidth * cosRot + halfHeight * sinRot;
	vertices[1].y = cy + halfWidth * sinRot - halfHeight * cosRot;
	vertices[1].z = 0.14f;
	vertices[1].z2 = 0.14f;
	vertices[1].flags = vertexFlags;
	vertices[1].u = (float)m_spriteWidth - 0.3f;
	vertices[1].v = 0.3f;

	vertices[2].x = cx + halfWidth * cosRot - halfHeight * sinRot;
	vertices[2].y = cy + halfWidth * sinRot + halfHeight * cosRot;
	vertices[2].z = 0.14f;
	vertices[2].z2 = 0.14f;
	vertices[2].flags = vertexFlags;
	vertices[2].u = (float)m_spriteWidth - 0.3f;
	vertices[2].v = (float)m_spriteHeight - 0.3f;

	vertices[3].x = cx - halfWidth * cosRot - halfHeight * sinRot;
	vertices[3].y = cy - halfWidth * sinRot + halfHeight * cosRot;
	vertices[3].z = 0.14f;
	vertices[3].z2 = 0.14f;
	vertices[3].flags = vertexFlags;
	vertices[3].u = 0.3f;
	vertices[3].v = (float)m_spriteHeight - 0.3f;

	Game::Functions::DrawQuad(
		m_options.alpha == 0xFF ? 0x28080 : 0xa180,
		(DWORD)sprite,
		vertices,
		0xff
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
