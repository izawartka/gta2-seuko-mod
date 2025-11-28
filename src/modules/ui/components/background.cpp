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
	Game::Camera* mainCamera = Game::Memory::GetMainCamera();
	if (!mainCamera) {
		spdlog::warn("UiModule::Background::Draw: Main camera is null, cannot get UI scale");
		return;
	}
	Game::SCR_f uiScale = mainCamera->uiScale;

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

	float x1 = Game::Utils::ToFloat(Game::Utils::Multiply(m_rect.x, uiScale));
	float y1 = Game::Utils::ToFloat(Game::Utils::Multiply(m_rect.y, uiScale));
	float x2 = Game::Utils::ToFloat(Game::Utils::Multiply(m_rect.x + m_rect.width, uiScale));
	float y2 = Game::Utils::ToFloat(Game::Utils::Multiply(m_rect.y + m_rect.height, uiScale));

	Game::GTAVertex vertices[4]{};
	vertices[0].x = x1,
	vertices[0].y = y1,
	vertices[0].z = 0.14f;
	vertices[0].z2 = 0.14f;
	vertices[0].flags = vertexFlags;
	vertices[0].u = 0.3f;
	vertices[0].v = 0.3f;

	vertices[1].x = x2,
	vertices[1].y = y1,
	vertices[1].z = 0.14f;
	vertices[1].z2 = 0.14f;
	vertices[1].flags = vertexFlags;
	vertices[1].u = 0.7f;
	vertices[1].v = 0.3f;

	vertices[2].x = x2;
	vertices[2].y = y2;
	vertices[2].z = 0.14f;
	vertices[2].z2 = 0.14f;
	vertices[2].flags = vertexFlags;
	vertices[2].u = 0.7f;
	vertices[2].v = 0.7f;

	vertices[3].x = x1;
	vertices[3].y = y2;
	vertices[3].z = 0.14f;
	vertices[3].z2 = 0.14f;
	vertices[3].flags = vertexFlags;
	vertices[3].u = 0.3f;
	vertices[3].v = 0.7f;

	Game::Functions::DrawQuad(
		m_options.alpha == 0xFF ? 0x28080 : 0x2a180,
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
