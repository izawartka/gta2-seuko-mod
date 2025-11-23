#include "background.h"

UiModule::Background::Background(Component* parent, const BackgroundOptions& options)
{
	SetOptions(options);

	if (parent) {
		parent->AddChild(this);
	}
}

void UiModule::Background::Draw()
{
	Game::S4* s4 = Game::Memory::GetS4();
	void* sprite = Game::Functions::GetSpriteTexture(
		s4,
		0,
		Game::SPRITE_TYPE_CODE_OBJ,
		309,
		Game::PALETTE_BASE_SPRITE,
		0
	);

	uint32_t vertexFlags = 0x00ffffff | (static_cast<uint32_t>(m_options.alpha) << 24);

	Game::GTAVertex vertices[4]{};
	vertices[0].x = Game::Utils::ToFloat(m_rect.x);
	vertices[0].y = Game::Utils::ToFloat(m_rect.y);
	vertices[0].z = 0.14f;
	vertices[0].z2 = 0.14f;
	vertices[0].flags = vertexFlags;
	vertices[0].u = 0.3f;
	vertices[0].v = 0.3f;

	vertices[1].x = Game::Utils::ToFloat(m_rect.x + m_rect.width);
	vertices[1].y = Game::Utils::ToFloat(m_rect.y);
	vertices[1].z = 0.14f;
	vertices[1].z2 = 0.14f;
	vertices[1].flags = vertexFlags;
	vertices[1].u = 0.7f;
	vertices[1].v = 0.3f;

	vertices[2].x = Game::Utils::ToFloat(m_rect.x + m_rect.width);
	vertices[2].y = Game::Utils::ToFloat(m_rect.y + m_rect.height);
	vertices[2].z = 0.14f;
	vertices[2].z2 = 0.14f;
	vertices[2].flags = vertexFlags;
	vertices[2].u = 0.7f;
	vertices[2].v = 0.7f;

	vertices[3].x = Game::Utils::ToFloat(m_rect.x);
	vertices[3].y = Game::Utils::ToFloat(m_rect.y + m_rect.height);
	vertices[3].z = 0.14f;
	vertices[3].z2 = 0.14f;
	vertices[3].flags = vertexFlags;
	vertices[3].u = 0.3f;
	vertices[3].v = 0.7f;

	Game::Functions::DrawQuad(
		m_options.alpha == 0xFF ? 0x28080 : 0xa180,
		(DWORD)sprite,
		vertices,
		0xff
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
