#include "ped-preview-segment.h"
#include "../root.h"

ModMenuModule::PedPreviewSegment::PedPreviewSegment()
{

}

ModMenuModule::PedPreviewSegment::~PedPreviewSegment()
{

}

void ModMenuModule::PedPreviewSegment::SetPedData(const Utils::SpawnPed::SpawnPedData& pedData)
{
	m_data = pedData;
	UpdateSpritePreview();
}

bool ModMenuModule::PedPreviewSegment::Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent)
{
	CreateSegment(menu, parent);

	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();
	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();

	// sprite preview
	UpdateSpritePreview();
	auto* spritePreviewMargin = uiRoot->AddComponent<UiModule::Margin>(
		m_vertCont,
		options.menuControllerOptions.createdSelectableOptions.markerOffsetX,
		options.menuSpacerHeight
	);
	m_spritePreview = uiRoot->AddComponent<UiModule::Sprite>(spritePreviewMargin, m_spritePreviewOptions);

	return true;
}

void ModMenuModule::PedPreviewSegment::Detach()
{
	m_spritePreview = nullptr;
	DestroySegment();
}

void ModMenuModule::PedPreviewSegment::UpdateSpritePreview()
{
	uint16_t spriteId = PED_SPRITE_PREVIEW_SPRITE_ID_BASE + static_cast<uint16_t>(m_data.graphicType) * PED_SPRITE_PREVIEW_SPRITE_ID_SHAPE_OFFSET;

	m_spritePreviewOptions = {
		Game::SPRITE_TYPE_PED,
		spriteId,
		Game::PALETTE_BASE_PED_REMAP,
		m_data.remap,
		PED_SPRITE_PREVIEW_SCALE,
		0.0f, // in radians
		255
	};

	if (m_spritePreview) {
		m_spritePreview->SetOptions(m_spritePreviewOptions);
	}
}
