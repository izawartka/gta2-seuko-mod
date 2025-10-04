#pragma once
#include "../common.h"
#include "../controller.h"
#include "../converter-support.h"
#include "../components/text.h"
#include "../../../events/draw.h"

namespace UiModule {
	struct VarTextControllerOptions {
		std::wstring prefix = L"";
		std::wstring suffix = L"";
		std::wstring nullText = L"N/A";
	};

	template <typename T>
	class VarTextController : public Controller, public Core::EventListenerSupport, public ConverterSupport<T> {
	public:
		VarTextController(Text* text, Core::Resolver<T> resolver, VarTextControllerOptions options = {}) {
			static_assert(std::is_copy_constructible<T>::value, "T must be copy-constructible");
			m_textComponent = text;
			m_options = options;
			m_resolver = resolver;

			m_textBuffer = m_options.nullText;
			UpdateText();
		}

		virtual ~VarTextController() {
			SetWatching(false);
		}

		void SetWatching(bool watching) {
			if (m_watching == watching) return;
			m_watching = watching;
			if (m_watching) {
				m_watched = Core::WatchManager::GetInstance()->Watch<PreDrawUIEvent, T>(
					m_resolver,
					this,
					&VarTextController<T>::OnValueUpdate
				);
			}
			else {
				Core::WatchManager::GetInstance()->Unwatch(m_watched);
				m_watched = nullptr;
			}
		}

		bool IsWatching() const { return m_watching; }

	protected:
		void UpdateText() {
			m_textComponent->SetText(m_options.prefix + m_textBuffer + m_options.suffix);
		}

		void OnValueUpdate(std::optional<T> oldValue, std::optional<T> newValue) {
			if (newValue.has_value()) {
				m_textBuffer = this->ConvertToString(newValue.value());
			}
			else {
				m_textBuffer = m_options.nullText;
			}
		}

		VarTextControllerOptions m_options;
		Text* m_textComponent = nullptr;
		Core::Resolver<T> m_resolver = nullptr;
		Core::Watched<T>* m_watched = nullptr;
		bool m_watching = false;
		std::wstring m_textBuffer = L"";
	};
}
