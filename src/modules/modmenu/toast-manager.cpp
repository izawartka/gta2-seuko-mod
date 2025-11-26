#include "toast-manager.h"
#include "root.h"

ModMenuModule::ToastManager* ModMenuModule::ToastManager::m_instance = nullptr;

ModMenuModule::ToastManager* ModMenuModule::ToastManager::GetInstance() {
	assert(m_instance && "ToastManager not initialized!");
	return m_instance;
}

ModMenuModule::ToastManager::ToastManager() {
	assert(!m_instance && "ToastManager instance already exists");
	m_instance = this;
}

ModMenuModule::ToastManager::~ToastManager() {
	m_instance = nullptr;
}

void ModMenuModule::ToastManager::Show(const Toast& toast) {
	if (toast.time == 0) {
		spdlog::warn("ToastManager::Show: Toast time is 0, toast will not be shown");
		return;
	}

	m_toasts.emplace_back(toast);
}

void ModMenuModule::ToastManager::Attach() {
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	m_mainContainer = uiRoot->AddComponent<UiModule::OverridePos>(nullptr, std::nullopt, options.toastPadding);
	m_toastListContainer = uiRoot->AddComponent<UiModule::VertCont>(m_mainContainer);

	AddEventListener<UiModule::PreUpdateUIEvent>(&ToastManager::OnPreUpdateUI);
}

void ModMenuModule::ToastManager::Detach() {
	RemoveEventListener<UiModule::PreUpdateUIEvent>();

	m_toasts.clear();
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	uiRoot->RemoveComponent(m_mainContainer, true);
	m_mainContainer = nullptr;
	m_toastListContainer = nullptr;
}

void ModMenuModule::ToastManager::OnPreUpdateUI(UiModule::PreUpdateUIEvent& event) {
	Update();
}

short ModMenuModule::ToastManager::GetToastTypeRemap(ToastType type)
{
	switch (type) {
	case ToastType::Info:
		return 0;
	case ToastType::Warning:
		return 4;
	case ToastType::Error:
		return 5;
	default:
		return 0;
	}
}

void ModMenuModule::ToastManager::Update() {
	if (m_mainContainer == nullptr || m_toastListContainer == nullptr) {
		spdlog::error("ToastManager::Update: Toast manager not properly initialized");
		return;
	}

	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	auto it = std::remove_if(
		m_toasts.begin(),
		m_toasts.end(),
		[uiRoot](const ToastItem& toast) {
			if (toast.time != 0) {
				return false;
			}

			if (toast.container != nullptr) {
				uiRoot->RemoveComponent(toast.container, true);
			}

			return true;
		}
	);
	m_toasts.erase(it, m_toasts.end());

	for(auto& toast : m_toasts) {
		toast.time--;
		if (toast.container != nullptr) continue;

		toast.container = uiRoot->AddComponent<UiModule::HorCentered>(m_toastListContainer);
		toast.text = uiRoot->AddComponent<UiModule::Text>(
			toast.container,
			toast.message,
			options.toastTextSize,
			GetToastTypeRemap(toast.type)
		);
	}
}
