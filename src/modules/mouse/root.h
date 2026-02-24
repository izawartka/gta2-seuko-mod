#pragma once
#include "common.h"
#include "mouse-manager.h"

namespace MouseModule {
	class RootModule : public Core::ModuleBase {
	public:
		RootModule();
		virtual ~RootModule() override;
		static RootModule* GetInstance();
		bool Attach();
		void Detach();

	private:
		static RootModule* m_instance;
		MouseManager m_mouseManager;
	};
}
