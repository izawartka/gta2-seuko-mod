#pragma once
#include "../common.h"
#include "../segment-base.h"
#include "../utils/toggle-native-cheat-utils.h"
#include "../cheats/native-cheats-keeper.h"

namespace ModMenuModule {
	struct ToggleNativeCheatSegmentData {
		size_t cheatAddress;
		Utils::ToggleNativeCheat::ToggleNativeCheatMode mode;
	};

	class ToggleNativeCheatSegment : public Segment<ToggleNativeCheatSegmentData> {
	public:
		ToggleNativeCheatSegment() = default;
		virtual ~ToggleNativeCheatSegment() = default;

		virtual std::optional<ToggleNativeCheatSegmentData> GetSegmentData() const override;
		virtual bool SetSegmentData(const ToggleNativeCheatSegmentData& data) override;

	private:
		virtual bool Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent) override;
		virtual void Detach() override;

		void CreateCheatController(NativeCheatCategory category);
		void DestroyCheatController();
		void OnCheatCategoryControllerSave(NativeCheatCategory newCategory);

		UiModule::SelectController<NativeCheatCategory>* m_cheatCategoryController = nullptr;
		UiModule::SelectController<size_t>* m_cheatController = nullptr;
		UiModule::Text* m_cheatText = nullptr;
		UiModule::MenuItemId m_cheatMenuItemId = -1;
		UiModule::SelectController<Utils::ToggleNativeCheat::ToggleNativeCheatMode>* m_modeController = nullptr;
	};
}
