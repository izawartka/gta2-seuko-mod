#include "gang-respect-segment.h"
#include "../root.h"
#include "../../../converters/gang-name.h"

static constexpr Game::SCR_f GANG_RESPECT_SEGMENT_ICON_SIZE = Game::Utils::FromFloat(16.0f);
static constexpr Game::SCR_f GANG_RESPECT_SEGMENT_ICON_MARGIN_TOP = Game::Utils::FromFloat(4.0f);
static constexpr Game::SCR_f GANG_RESPECT_SEGMENT_ICON_MARGIN_RIGHT = Game::Utils::FromFloat(4.0f);
static constexpr uint16_t GANG_RESPECT_SEGMENT_BASE_SPITE_ID = 63; // first gang logo sprite

ModMenuModule::GangRespectSegment::GangRespectSegment()
{

}

ModMenuModule::GangRespectSegment::GangRespectSegment(Game::GangRespect* gang)
{
	m_gang = gang;
}

ModMenuModule::GangRespectSegment::~GangRespectSegment()
{

}

void ModMenuModule::GangRespectSegment::SetGang(Game::GangRespect* gang)
{
	if (IsAttached()) {
		spdlog::error("GangRespectSegment: Cannot change gang while segment is attached");
		return;
	}

	m_gang = gang;
}

bool ModMenuModule::GangRespectSegment::Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent)
{
	CreateSegment(menu, parent);

	if (!m_gang) {
		spdlog::error("GangRespectSegment: Cannot attach, gang not set");
		return false;
	}

	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	// container
	auto* respectCont = m_menuController->CreateItem<UiModule::HorCont>(m_vertCont);

	// icon
	auto* iconCont = uiRoot->AddComponent<UiModule::VertCont>(respectCont);
	uiRoot->AddComponent<UiModule::Spacer>(iconCont, 0, GANG_RESPECT_SEGMENT_ICON_MARGIN_TOP);

	UiModule::IconSpriteOptions iconOptions = {};
	iconOptions.spriteType = Game::SPRITE_TYPE_USER;
	iconOptions.spriteId = GANG_RESPECT_SEGMENT_BASE_SPITE_ID + m_gang->spritesOffset;
	iconOptions.palette = Game::PALETTE_BASE::PALETTE_BASE_SPRITE;
	iconOptions.width = GANG_RESPECT_SEGMENT_ICON_SIZE;
	iconOptions.height = GANG_RESPECT_SEGMENT_ICON_SIZE;
	iconOptions.keepAspectRatio = true;
	uiRoot->AddComponent<UiModule::IconSprite>(iconCont, iconOptions);

	// right margin
	uiRoot->AddComponent<UiModule::Spacer>(respectCont, GANG_RESPECT_SEGMENT_ICON_MARGIN_RIGHT, 0);

	// respect
	std::wstring gangName = GangNameConverter::ConvertToString(m_gang);
	auto* respectText = uiRoot->AddComponent<UiModule::Text>(respectCont, L"", options.textSize);
	auto* respectController = m_menuController->CreateLatestItemController<UiModule::VarTextEditableController<char>>(
		respectText,
		std::bind(&GangRespectSegment::GetGangRespectPtr, this),
		UiModule::VarTextEditableControllerOptions{ gangName + L" respect: #", L"#" }
	);

	return true;
}

void ModMenuModule::GangRespectSegment::Detach()
{
	DestroySegment();
}

char* ModMenuModule::GangRespectSegment::GetGangRespectPtr()
{
	Game::Player* player = Game::Utils::GetPlayer();
	if(!player) {
		spdlog::error("GangRespectSegment: Cannot get player");
		return nullptr;
	}

	return &m_gang->respectArr[player->gangsRespectIndex];
}
