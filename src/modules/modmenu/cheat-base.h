#pragma once
#include "common.h"
#include "persistence-manager.h"
#include <vector>
#include <functional>

namespace ModMenuModule {
	class CheatBase {
	public:
		virtual ~CheatBase() = default;
	protected:
		CheatBase(std::string persistenceKey) {
			m_persistenceKey = persistenceKey;
		}

		virtual void OnFirstEnable() {}
		virtual void OnEnable() {}
		virtual void OnDisable() {}

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
		}
		virtual bool IsEnabled() const final { return m_enabled; }

	private:
		friend class RootModule;
		virtual bool Attach() final {
			if (m_persistenceKey.empty()) return true;
			PersistenceManager* persistence = PersistenceManager::GetInstance();
			bool enabled = persistence->Load(m_persistenceKey, false);
			SetEnabled(enabled);
			return true;
		}

		virtual void Detach() final {
			bool enabled = IsEnabled();
			SetEnabled(false);
			if (m_persistenceKey.empty()) return;
			PersistenceManager* persistence = PersistenceManager::GetInstance();
			persistence->Save(m_persistenceKey, enabled);
		}

		bool m_firstEnableDone = false;
		bool m_enabled = false;
		std::string m_persistenceKey = "";
	};
}
