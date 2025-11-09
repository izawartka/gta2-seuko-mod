#pragma once
#include "common.h"
#include "events/cheat-state.h"

namespace ModMenuModule {
	class CheatBase {
	public:
		virtual ~CheatBase() = default;

		virtual void SetEnabled(bool enabled) final {
			if (m_enabled == enabled) return;
			m_enabled = enabled;
			if (enabled) {
				if (!m_firstEnableDone) {
					OnFirstEnable();
					m_firstEnableDone = true;
				}

				OnEnable();
			}
			else {
				OnDisable();
			}

			Core::EventManager* eventManager = Core::EventManager::GetInstance();
			ModMenuModule::CheatStateEvent event(typeid(*this), enabled);
			eventManager->Dispatch(event);
		}
		virtual bool IsEnabled() const final { return m_enabled; }
		virtual bool AutoEnableOnAttach() const { return false; }

	protected:
		CheatBase(std::string persistenceKey) {
			m_persistenceKey = persistenceKey;
		}

		virtual void OnFirstEnable() {}
		virtual void OnEnable() {}
		virtual void OnDisable() {}

	private:
		friend class RootModule;
		virtual bool Attach() final {
			if (AutoEnableOnAttach()) {
				SetEnabled(true);
				return true;
			}

			PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();
			bool enabled = persistence->Load(m_persistenceKey, false);
			SetEnabled(enabled);
			return true;
		}

		virtual void Detach() final {
			bool enabled = IsEnabled();
			SetEnabled(false);
			if (m_persistenceKey.empty()) return;
			PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();
			persistence->Save(m_persistenceKey, enabled);
		}

		bool m_firstEnableDone = false;
		bool m_enabled = false;
		std::string m_persistenceKey = "";
	};
}
