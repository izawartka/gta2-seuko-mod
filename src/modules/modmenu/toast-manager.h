#pragma once
#include "common.h"

namespace ModMenuModule {
	enum class ToastType {
		Info,
		Warning,
		Error
	};

	struct Toast {
		std::wstring message;
		ToastType type = ToastType::Info;
		unsigned short time = 120; // in frames
	};

	class ToastManager : public Core::EventListenerSupport {
	public:
		static ToastManager* GetInstance();

		void Show(const Toast& toast);

	private:
		friend class RootModule;
		ToastManager();
		virtual ~ToastManager();
		ToastManager(const ToastManager&) = delete;
		ToastManager& operator=(const ToastManager&) = delete;

		void Attach();
		void Detach();

		static ToastManager* m_instance;

		void OnPreUpdateUI(UiModule::PreUpdateUIEvent& event);
		static short GetToastTypeRemap(ToastType type);
		void Update();

		struct ToastItem : public Toast {
			UiModule::HorCentered* container = nullptr;
			UiModule::Text* text = nullptr;

			ToastItem(const Toast& toast)
				: Toast(toast), container(nullptr), text(nullptr) {
			}
		};

		std::vector<ToastItem> m_toasts = {};
		UiModule::OverridePos* m_mainContainer = nullptr;
		UiModule::VertCont* m_toastListContainer = nullptr;
	};
}