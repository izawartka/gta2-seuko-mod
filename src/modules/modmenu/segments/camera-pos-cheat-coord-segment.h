#pragma once
#include "../common.h"
#include "../segment-base.h"
#include "../cheats/camera/camera-pos.h"

namespace ModMenuModule {
	class CameraPosCheatCoordSegment : public SegmentBase {
	public:
		CameraPosCheatCoordSegment(const std::wstring& coordLabel, size_t coordIndex);
		virtual ~CameraPosCheatCoordSegment();

		virtual bool Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent) override;
		virtual void Detach() override;

		virtual void OnShow() override;
		virtual void OnHide() override;

	private:
		static const CameraPosCheatCoordinate* GetCoordinate(const CameraPosCheatOptions& cheatOptions, size_t coordIndex);
		static Game::SCR_f GetPosCoordinate(const std::optional<CameraPosCheatPosition>& position, size_t coordIndex);

		CameraPosCheatCoordinate GetCurrentCoordinateOptions() const;
		void OnModeChange(const std::optional<CameraPosCheatMode>& prevMode, const std::optional<CameraPosCheatMode>& newMode);
		void SetValueVisible(bool visible);

		std::wstring m_coordLabel;
		size_t m_coordIndex;
		UiModule::HorCont* m_horCont = nullptr;
		UiModule::VarTextEditableController<Game::SCR_f, Game::SCR_f>* m_valueController = nullptr;
		UiModule::MenuItemId m_modeMenuItemId = -1;
		UiModule::MenuItemId m_valueMenuItemId = -1;
		Core::Resolver<CameraPosCheatMode> m_modeResolver;
		Core::Watched<CameraPosCheatMode, CameraPosCheatMode>* m_watchedMode = nullptr;
	};
}
