#pragma once
#include "../common.h"
#include "../cheat-base.h"
#include "../utils/spawn-ped-utils.h"

namespace ModMenuModule {
	using TempPedTemplateId = size_t;

	struct PedTemplate {
		std::wstring name = L"";
		Utils::SpawnPed::SpawnPedData data = {};
		bool isBuiltin = false;
		bool isSaved = false;
	};

	class PedTemplatesCheat : public CheatBase {
	public:
		PedTemplatesCheat();
		virtual ~PedTemplatesCheat() override;
		static PedTemplatesCheat* GetInstance();

		bool SaveTemplate(const std::wstring& name, const Utils::SpawnPed::SpawnPedData& data);
		bool SaveTemplate(TempPedTemplateId tempTemplateId);
		bool SaveTemplate(TempPedTemplateId tempTemplateId, const std::wstring& customName);
		// pointers may be invalidated on state change
		const PedTemplate* GetTemplate(const std::wstring& name) const;
		bool DeleteTemplate(const std::wstring& name);
		bool RenameTemplate(const std::wstring& oldName, const std::wstring& newName);
		const std::vector<std::wstring>& GetTemplatesList();

		TempPedTemplateId FindOrCreateTempTemplate(const std::string& persistenceKey, const std::wstring& baseTemplateName);
		TempPedTemplateId FindTempTemplate(const std::string& persistenceKey);
		std::shared_ptr<PedTemplate> GetTempTemplate(TempPedTemplateId id) const;
		void DeleteTempTemplate(TempPedTemplateId id);

	private:
		struct TempPedTemplate {
			std::string persistenceKey = "";
			TempPedTemplateId id = 0;
			std::shared_ptr<PedTemplate> pedTemplatePtr = nullptr;
		};

		virtual void OnFirstEnable() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;
		virtual bool AutoEnableOnAttach() const override { return true; }

		std::unordered_map<TempPedTemplateId, TempPedTemplate>::iterator FindTempTemplateByPersistenceKey(const std::string& persistenceKey);

		void SaveToPersistence() const;
		void LoadFromPersistence();
		static bool ConvertPersistence(std::unique_ptr<uint8_t[]>& dataPtr, size_t& dataSize, uint8_t version);
		void RefreshSortedCache();

		static PedTemplatesCheat* m_instance;
		std::unordered_map<std::wstring, PedTemplate> m_templates = {};
		std::unordered_map<TempPedTemplateId, TempPedTemplate> m_tempTemplates = {};
		TempPedTemplateId m_nextTempTemplateId = 1;
		std::vector<std::wstring> m_sortedCache = {};
		bool m_cacheDirty = true;
	};
}
