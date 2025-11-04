#pragma once
#include "common.h"
#include "bind-manager.h"

namespace KeyBindingModule {
	class RootModule : public Core::ModuleBase {
	public:
		RootModule();
		virtual ~RootModule() override;
		static RootModule* GetInstance();
		bool Attach();
		void Detach();

	private:
		static RootModule* m_instance;

		BindManager m_bindManager;
	};
}
