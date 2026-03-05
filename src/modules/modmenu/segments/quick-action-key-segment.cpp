#include "quick-action-key-segment.h"
#include "../root.h"

ModMenuModule::QuickActionKeySegment::QuickActionKeySegment(QuickActionId actionId)
{
	m_actionId = actionId;
}

ModMenuModule::QuickActionKeySegment::~QuickActionKeySegment()
{

}

std::optional<ModMenuModule::QuickActionKeySegmentData> ModMenuModule::QuickActionKeySegment::GetSegmentData() const
{
	if (!m_keyController) {
		spdlog::error("Cannot get segment data: controller is not initialized.");
		return std::nullopt;
	}

	auto keyOpt = m_keyController->GetValue();

	if (!keyOpt.has_value() || keyOpt.value().keyCode == Game::KeyCode::DIK_NONE) {
		spdlog::warn("Cannot get segment data: no key bind set");
		return std::nullopt;
	}

	return QuickActionKeySegmentData{
		keyOpt.value()
	};
}

bool ModMenuModule::QuickActionKeySegment::SetSegmentData(const QuickActionKeySegmentData& data)
{
	if (!m_keyController) {
		spdlog::error("Cannot set segment data: controller is not initialized.");
		return false;
	}

	m_keyController->SetValue(data.key);
	UpdateKeyTakenText();
	return true;
}

bool ModMenuModule::QuickActionKeySegment::IsKeyTaken() const
{
	return m_keyTakenText != nullptr;
}

bool ModMenuModule::QuickActionKeySegment::Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent)
{
	CreateSegment(menu, parent);

	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	// key
	auto keyText = m_menuController->CreateItem<UiModule::Text>(m_vertCont, L"", options.textSize);
	m_keyController = m_menuController->CreateLatestItemController<UiModule::KeyChangeController>(
		keyText,
		std::nullopt,
		UiModule::KeyChangeControllerOptions{ L"Key: #", L"#" }
	);
	m_keyController->SetSaveCallback(std::bind(&QuickActionKeySegment::UpdateKeyTakenText, this));
	m_keyController->SetEditStateChangeCallback(std::bind(&QuickActionKeySegment::OnKeyEditStateChange, this, std::placeholders::_1));

	m_keyTakenContainer = uiRoot->AddComponent<UiModule::VertCont>(m_vertCont);

	return true;
}

void ModMenuModule::QuickActionKeySegment::Detach()
{
	m_keyController = nullptr;
	m_keyTakenContainer = nullptr;
	m_keyTakenText = nullptr;

	DestroySegment();
}

void ModMenuModule::QuickActionKeySegment::OnShow()
{
	AddEventListener<KeyBindingModule::KeyBindUpdateEvent>(&QuickActionKeySegment::OnKeyBindUpdate);
	UpdateKeyTakenText();
}

void ModMenuModule::QuickActionKeySegment::OnHide()
{
	RemoveEventListener<KeyBindingModule::KeyBindUpdateEvent>();
}

void ModMenuModule::QuickActionKeySegment::OnKeyBindUpdate(KeyBindingModule::KeyBindUpdateEvent& event)
{
	UpdateKeyTakenText();
}

void ModMenuModule::QuickActionKeySegment::OnKeyEditStateChange(bool editing)
{
	QuickActionManager* quickActionManager = QuickActionManager::GetInstance();
	quickActionManager->SetListen(!editing);

	if (editing) {
		SetShowKeyTakenText(false);
	}
}

void ModMenuModule::QuickActionKeySegment::SetShowKeyTakenText(bool show)
{
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	if (show) {
		if (m_keyTakenText) return;
		const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();
		Game::SCR_f marginX = options.menuControllerOptions.createdSelectableOptions.markerOffsetX;
		auto* keyTakenMargin = uiRoot->AddComponent<UiModule::Margin>(m_keyTakenContainer, marginX, 0);
		m_keyTakenText = uiRoot->AddComponent<UiModule::Text>(keyTakenMargin, L"", options.textSize);
		m_keyTakenText->SetRemap(4); // orange
	}
	else {
		if (!m_keyTakenText) return;
		uiRoot->RemoveComponent(m_keyTakenText->GetParent(), true);
		m_keyTakenText = nullptr;
	}
}

void ModMenuModule::QuickActionKeySegment::UpdateKeyTakenText()
{
	if (!m_keyController) return;
	if (!m_keyTakenContainer) return;

	const auto& keyOpt = m_keyController->GetValue();
	if (!keyOpt.has_value()) {
		SetShowKeyTakenText(false);
		return;
	}

	const auto& key = keyOpt.value();
	std::optional<QuickActionId> ignoreActionId = m_actionId == -1 ? std::nullopt : std::optional<QuickActionId>(m_actionId);
	bool isInUse = QuickActionManager::CheckKeyIsInUse(key, ignoreActionId);
	if (isInUse) {
		SetShowKeyTakenText(true);
		m_keyTakenText->SetText(L"(Key already in use)");
	}
	else {
		SetShowKeyTakenText(false);
	}
}
