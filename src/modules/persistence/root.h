#pragma once
#include "common.h"
#include "persistence-manager.h"

namespace PersistenceModule {
	class RootModule : public Core::ModuleBase {
	public:
		RootModule();
		virtual ~RootModule() override;
		static RootModule* GetInstance();
		bool Attach();
		void Detach();

	private:
		static RootModule* m_instance;
		PersistenceManager m_persistenceManager;
	};
}
