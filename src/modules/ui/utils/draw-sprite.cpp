#include "draw-sprite.h"

void UiModule::Utils::DrawRotatedSprite(
	Game::SPRITE_TYPE type, 
	uint16_t id, 
	Game::PALETTE_BASE palette, 
	uint16_t remap,
	float cx,
	float cy,
	int spriteWidth,
	int spriteHeight,
	float scale,
	float rotation, 
	uint8_t alpha
)
{
	Game::S4* s4 = Game::Memory::GetS4();
	void* sprite = Game::Functions::GetSpriteTexture(
		s4,
		0,
		type,
		id,
		palette,
		remap
	);

	float halfWidth = scale * (float)spriteWidth / 2.0f;
	float halfHeight = scale * (float)spriteHeight / 2.0f;
	float cosRot = cos(rotation);
	float sinRot = sin(rotation);

	uint32_t zBits = 0x38D1B717u;
	float z;
	std::memcpy(&z, &zBits, sizeof(zBits));

	uint32_t vertexFlags = 0x00ffffff | (static_cast<uint32_t>(alpha) << 24);

	Game::GTAVertex vertices[4]{};
	vertices[0].x = cx - halfWidth * cosRot + halfHeight * sinRot;
	vertices[0].y = cy - halfWidth * sinRot - halfHeight * cosRot;
	vertices[0].z = z;
	vertices[0].z2 = z;
	vertices[0].flags = vertexFlags;
	vertices[0].field_0x14 = 0;
	vertices[0].u = 0.3f;
	vertices[0].v = 0.3f;

	vertices[1].x = cx + halfWidth * cosRot + halfHeight * sinRot;
	vertices[1].y = cy + halfWidth * sinRot - halfHeight * cosRot;
	vertices[1].z = z;
	vertices[1].z2 = z;
	vertices[1].flags = vertexFlags;
	vertices[1].field_0x14 = 0;
	vertices[1].u = (float)spriteWidth - 0.3f;
	vertices[1].v = 0.3f;

	vertices[2].x = cx + halfWidth * cosRot - halfHeight * sinRot;
	vertices[2].y = cy + halfWidth * sinRot + halfHeight * cosRot;
	vertices[2].z = z;
	vertices[2].z2 = z;
	vertices[2].flags = vertexFlags;
	vertices[2].field_0x14 = 0;
	vertices[2].u = (float)spriteWidth - 0.3f;
	vertices[2].v = (float)spriteHeight - 0.3f;

	vertices[3].x = cx - halfWidth * cosRot - halfHeight * sinRot;
	vertices[3].y = cy - halfWidth * sinRot + halfHeight * cosRot;
	vertices[3].z = z;
	vertices[3].z2 = z;
	vertices[3].flags = vertexFlags;
	vertices[3].field_0x14 = 0;
	vertices[3].u = 0.3f;
	vertices[3].v = (float)spriteHeight - 0.3f;

	Game::Functions::DrawQuad(
		alpha == 0xFF ? 0x28080 : 0x2a180,
		(DWORD)sprite,
		vertices,
		0xff
	);
}

void UiModule::Utils::DrawSprite(
	Game::SPRITE_TYPE type, 
	uint16_t id, 
	Game::PALETTE_BASE palette, 
	uint16_t remap, 
	float centerX, 
	float centerY, 
	int spriteWidth, 
	int spriteHeight, 
	float scale, 
	uint8_t alpha
)
{
	float halfWidth = (static_cast<float>(spriteWidth) * scale) / 2.0f;
	float halfHeight = (static_cast<float>(spriteHeight) * scale) / 2.0f;
	float x1 = centerX - halfWidth;
	float y1 = centerY - halfHeight;
	float x2 = centerX + halfWidth;
	float y2 = centerY + halfHeight;

	DrawSprite(
		type,
		id,
		palette,
		remap,
		x1,
		y1,
		x2,
		y2,
		spriteWidth,
		spriteHeight,
		alpha
	);
}

void UiModule::Utils::DrawSprite(
	Game::SPRITE_TYPE type, 
	uint16_t id, 
	Game::PALETTE_BASE palette, 
	uint16_t remap, 
	float x1, 
	float y1, 
	float x2, 
	float y2, 
	int spriteWidth, 
	int spriteHeight, 
	uint8_t alpha
)
{
	Game::S4* s4 = Game::Memory::GetS4();
	void* sprite = Game::Functions::GetSpriteTexture(
		s4,
		0,
		type,
		id,
		palette,
		remap
	);

	uint32_t zBits = 0x38D1B717u;
	float z;
	std::memcpy(&z, &zBits, sizeof(zBits));

	uint32_t vertexFlags = 0x00ffffff | (static_cast<uint32_t>(alpha) << 24);

	Game::GTAVertex vertices[4]{};
	vertices[0].x = x1;
	vertices[0].y = y1;
	vertices[0].z = z;
	vertices[0].z2 = z;
	vertices[0].flags = vertexFlags;
	vertices[0].field_0x14 = 0;
	vertices[0].u = 0.3f;
	vertices[0].v = 0.3f;

	vertices[1].x = x2;
	vertices[1].y = y1;
	vertices[1].z = z;
	vertices[1].z2 = z;
	vertices[1].flags = vertexFlags;
	vertices[1].field_0x14 = 0;
	vertices[1].u = (float)spriteWidth - 0.3f;
	vertices[1].v = 0.3f;

	vertices[2].x = x2;
	vertices[2].y = y2;
	vertices[2].z = z;
	vertices[2].z2 = z;
	vertices[2].flags = vertexFlags;
	vertices[2].field_0x14 = 0;
	vertices[2].u = (float)spriteWidth - 0.3f;
	vertices[2].v = (float)spriteHeight - 0.3f;

	vertices[3].x = x1;
	vertices[3].y = y2;
	vertices[3].z = z;
	vertices[3].z2 = z;
	vertices[3].flags = vertexFlags;
	vertices[3].field_0x14 = 0;
	vertices[3].u = 0.3f;
	vertices[3].v = (float)spriteHeight - 0.3f;

	Game::Functions::DrawQuad(
		alpha == 0xFF ? 0x28080 : 0x2a180,
		(DWORD)sprite,
		vertices,
		0xff
	);
}
