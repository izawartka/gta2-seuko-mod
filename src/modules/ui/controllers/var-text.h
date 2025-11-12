#pragma once
#include "../common.h"
#include "menu-item.h"
#include "../converter-support.h"
#include "../components/text.h"
#include "../../../events/draw.h"

namespace UiModule {
	struct VarTextControllerOptions {
		std::wstring prefix = L"";
		std::wstring suffix = L"";
		std::wstring nullText = L"N/A";
	};

	template <typename ValueT, typename ResRetT = typename Core::DefaultResRetT<ValueT>::type>
	class VarTextController : public MenuItemController, public Core::EventListenerSupport, public ConverterSupport<ValueT> {
	public:
		VarTextController(Text* text, Core::Resolver<ResRetT> resolver, VarTextControllerOptions options = {}) {
			static_assert(std::is_copy_constructible<ValueT>::value, "ValueT must be copy-constructible");
			m_textComponent = text;
			m_options = options;
			m_resolver = resolver;

			m_textBuffer = m_options.nullText;
			UpdateText();
		}

		virtual ~VarTextController() {
			SetWatching(false);
		}

		virtual void SetWatching(bool watching) override {
			if (m_watching == watching) return;
			m_watching = watching;
			if (m_watching) {
				m_watched = Core::WatchManager::GetInstance()->Watch<PreDrawUIEvent, ValueT, ResRetT>(
					m_resolver,
					this,
					&VarTextController<ValueT, ResRetT>::OnValueUpdate
				);
			}
			else {
				Core::WatchManager::GetInstance()->Unwatch(m_watched);
				m_watched = nullptr;
			}
		}

	protected:
		virtual void OnConverterChanged() override {
			UpdateTextBuffer();
			UpdateText();
		}

		void UpdateTextBuffer() {
			if (m_value.has_value()) {
				m_textBuffer = this->ConvertToString(m_value.value());
			}
			else {
				m_textBuffer = m_options.nullText;
			}
		}

		void UpdateText() {
			m_textComponent->SetText(m_options.prefix + m_textBuffer + m_options.suffix);
		}

		void OnValueUpdate(std::optional<ValueT> oldValue, std::optional<ValueT> newValue) {
			m_value = newValue;
			UpdateTextBuffer();
			UpdateText();
		}

		VarTextControllerOptions m_options;
		Text* m_textComponent = nullptr;
		Core::Resolver<ResRetT> m_resolver = nullptr;
		Core::Watched<ValueT, ResRetT>* m_watched = nullptr;
		std::optional<ValueT> m_value = std::nullopt;
		std::wstring m_textBuffer = L"";
	};
}
