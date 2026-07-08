#pragma once
#include "../common.h"
#include "../segment-base.h"

namespace ModMenuModule {
	class GangRespectSegment : public SegmentBase {
	public:
		GangRespectSegment();
		GangRespectSegment(Game::GangRespect* gang);
		virtual ~GangRespectSegment();

		void SetGang(Game::GangRespect* gang);

	private:
		virtual bool Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent) override;
		virtual void Detach() override;

		char* GetGangRespectPtr();

		Game::GangRespect* m_gang = nullptr;
	};
}
