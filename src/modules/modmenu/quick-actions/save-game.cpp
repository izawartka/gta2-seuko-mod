#include "save-game.h"
#include "../utils/save-game.h"
#include "../quick-action-registry.h"

static const std::string typeId = "ModMenu_SaveGame";
static const std::wstring typeLabel = L"Save game";

ModMenuModule::SaveGameAction::SaveGameAction()
{
}

ModMenuModule::SaveGameAction::~SaveGameAction()
{
}

const std::string& ModMenuModule::SaveGameAction::GetTypeId()
{
	return typeId;
}

const std::wstring& ModMenuModule::SaveGameAction::GetTypeLabel()
{
	return typeLabel;
}

void ModMenuModule::SaveGameAction::Execute()
{
	ModMenuModule::Utils::SaveGame();
}

const std::wstring& ModMenuModule::SaveGameAction::GetLabel() const
{
	return typeLabel;
}

REGISTER_QUICK_ACTION(SaveGameAction)
