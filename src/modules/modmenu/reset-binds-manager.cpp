#include "reset-binds-manager.h"
#include "root.h"

ModMenuModule::ResetBindsManager* ModMenuModule::ResetBindsManager::m_instance = nullptr;

ModMenuModule::ResetBindsManager* ModMenuModule::ResetBindsManager::GetInstance() {
	assert(m_instance, "ResetBindsManager not initialized!");
	return m_instance;
}

ModMenuModule::ResetBindsManager::ResetBindsManager() {
	assert(!m_instance, "ResetBindsManager instance already exists");
	m_instance = this;
}

ModMenuModule::ResetBindsManager::~ResetBindsManager() {
	m_instance = nullptr;
}

void ModMenuModule::ResetBindsManager::Reset() {
	spdlog::info("Resetting Mod Menu key binds to default values");
	ModMenuModule::ModMenuOptions options = ModMenuModule::RootModule::GetInstance()->GetOptions();
	auto* bindManager = KeyBindingModule::BindManager::GetInstance();
	bindManager->SetBind(options.keyBindToggleMenuName, options.keyBindToggleMenuDefault);
	UiModule::StandardBindsSupportOptions uiBindOptions = options.menuControllerOptions.keyBindOptions;
	bindManager->SetBind(uiBindOptions.keyBindNextName, uiBindOptions.keyBindNextDefault);
	bindManager->SetBind(uiBindOptions.keyBindPrevName, uiBindOptions.keyBindPrevDefault);
	bindManager->SetBind(uiBindOptions.keyBindActionName, uiBindOptions.keyBindActionDefault);
}

void ModMenuModule::ResetBindsManager::Attach() {
	AddEventListener<KeyDownEvent>(&ResetBindsManager::OnKeyDown);
	AddEventListener<KeyUpEvent>(&ResetBindsManager::OnKeyUp);
	AddEventListener<PreDrawUIEvent>(&ResetBindsManager::OnPreDrawUI);
	AddEventListener<PreGameEndEvent>(&ResetBindsManager::OnPreGameEnd);
}

void ModMenuModule::ResetBindsManager::Detach() {
	RemoveEventListener<PreGameEndEvent>();
	RemoveEventListener<PreDrawUIEvent>();
	RemoveEventListener<KeyUpEvent>();
	RemoveEventListener<KeyDownEvent>();
	m_resetKeyHoldFrames = 0;
}

void ModMenuModule::ResetBindsManager::OnKeyDown(KeyDownEvent& event) {
	ModMenuModule::ModMenuOptions options = ModMenuModule::RootModule::GetInstance()->GetOptions();
	KeyBindingModule::Key key = KeyBindingModule::Key::FromKeyDownEvent(event);
	if (key == options.keyBindToggleMenuDefault) {
		m_resetKeyHoldFrames++;
	}
}

void ModMenuModule::ResetBindsManager::OnKeyUp(KeyUpEvent& event)
{
	ModMenuModule::ModMenuOptions options = ModMenuModule::RootModule::GetInstance()->GetOptions();
	Game::KeyCode keyCode = event.GetKeyCode();
	if (keyCode == options.keyBindToggleMenuDefault.keyCode) {
		m_resetKeyHoldFrames = 0;
	}
}

void ModMenuModule::ResetBindsManager::OnPreDrawUI(PreDrawUIEvent& event)
{
	if (m_resetKeyHoldFrames == 0) return;
	m_resetKeyHoldFrames++;

	ModMenuModule::ModMenuOptions options = ModMenuModule::RootModule::GetInstance()->GetOptions();
	if (m_resetKeyHoldFrames > options.resetBindsHoldTime) {
		Reset();
		m_resetKeyHoldFrames = 0;
	}
}

void ModMenuModule::ResetBindsManager::OnPreGameEnd(PreGameEndEvent& event)
{
	m_resetKeyHoldFrames = 0;
}

