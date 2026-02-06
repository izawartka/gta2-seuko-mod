#include "reset-binds-manager.h"
#include "root.h"

ModMenuModule::ResetBindsManager* ModMenuModule::ResetBindsManager::m_instance = nullptr;

ModMenuModule::ResetBindsManager* ModMenuModule::ResetBindsManager::GetInstance() {
	assert(m_instance && "ResetBindsManager not initialized!");
	return m_instance;
}

ModMenuModule::ResetBindsManager::ResetBindsManager() {
	assert(!m_instance && "ResetBindsManager instance already exists");
	m_instance = this;
}

ModMenuModule::ResetBindsManager::~ResetBindsManager() {
	m_instance = nullptr;
}

void ModMenuModule::ResetBindsManager::Reset() {
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();
	auto* bindManager = KeyBindingModule::BindManager::GetInstance();
	bindManager->SetBind(options.keyBindToggleMenuName, options.keyBindToggleMenuDefault);
	UiModule::StandardBindsSupportOptions uiBindOptions = options.menuControllerOptions.keyBindOptions;
	bindManager->SetBind(uiBindOptions.keyBindNextName, uiBindOptions.keyBindNextDefault);
	bindManager->SetBind(uiBindOptions.keyBindPrevName, uiBindOptions.keyBindPrevDefault);
	bindManager->SetBind(uiBindOptions.keyBindActionName, uiBindOptions.keyBindActionDefault);\

	spdlog::info("Reset Mod Menu key binds to default values");
	ToastManager::GetInstance()->Show({ L"Reset Mod Menu key binds" });
}

void ModMenuModule::ResetBindsManager::Attach() {
	AddEventListener<KeyDownEvent>(&ResetBindsManager::OnKeyDown);
	AddEventListener<KeyUpEvent>(&ResetBindsManager::OnKeyUp);
	AddEventListener<UiModule::PreUpdateUIEvent>(&ResetBindsManager::OnPreUpdateUI);
	AddEventListener<PreGameEndEvent>(&ResetBindsManager::OnPreGameEnd);
}

void ModMenuModule::ResetBindsManager::Detach() {
	RemoveEventListener<PreGameEndEvent>();
	RemoveEventListener<UiModule::PreUpdateUIEvent>();
	RemoveEventListener<KeyUpEvent>();
	RemoveEventListener<KeyDownEvent>();
	m_resetKeyHoldFrames = 0;
}

void ModMenuModule::ResetBindsManager::OnKeyDown(KeyDownEvent& event) {
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();
	KeyBindingModule::Key key = KeyBindingModule::Key::FromKeyboardEvent(event);
	if (key == options.keyBindToggleMenuDefault) {
		m_resetKeyHoldFrames++;
	}
}

void ModMenuModule::ResetBindsManager::OnKeyUp(KeyUpEvent& event)
{
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();
	Game::KeyCode keyCode = event.GetKeyCode();
	if (keyCode == options.keyBindToggleMenuDefault.keyCode) {
		m_resetKeyHoldFrames = 0;
	}
}

void ModMenuModule::ResetBindsManager::OnPreUpdateUI(UiModule::PreUpdateUIEvent& event)
{
	if (m_resetKeyHoldFrames == 0) return;
	m_resetKeyHoldFrames++;

	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	if (m_resetKeyHoldFrames >= options.resetBindsHoldTime) {
		Reset();
		m_resetKeyHoldFrames = 0;
	}
	else if (m_resetKeyHoldFrames % options.resetBindsToastInterval == 0) {
		ModMenuModule::ToastManager::GetInstance()->Show({
			L"Keep holding to reset Mod Menu key binds...", 
			ToastType::Info, 
			static_cast<unsigned short>(options.resetBindsToastInterval)
		});
	}
}

void ModMenuModule::ResetBindsManager::OnPreGameEnd(PreGameEndEvent& event)
{
	m_resetKeyHoldFrames = 0;
}

