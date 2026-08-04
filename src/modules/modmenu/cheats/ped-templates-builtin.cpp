#include "ped-templates.h"

namespace ModMenuModule {
	const std::vector<PedTemplate> builtInPedTemplatesArr = {
		{ L"Dummy", {
			Game::PED_GRAPHIC_TYPE_DUMMY,
			Game::PED_REMAP_DUMMY_27,
			static_cast<Game::WEAPON_INDEX>(-1),
			50,
			Game::PED_OCCUPATION_DUMMY,
			Game::PED_THREAT_SEARCH_AREA,
			Game::PED_THREAT_REACTION_RUN_AWAY,
			Game::PED_OBJECTIVE_NO_OBJ,
			9999,
			{},
			Game::PED_STATE_WALKING,
			Game::PED_STATE2_WALKING,
			static_cast<Game::PED_BIT_STATE>(1),
			static_cast<Game::PED_BIT_STATE2>(3),
			-1
		}, true, true},
		{ L"Following car in car", {
			Game::PED_GRAPHIC_TYPE_DUMMY,
			Game::PED_REMAP_DUMMY_37,
			static_cast<Game::WEAPON_INDEX>(-1),
			50,
			Game::PED_OCCUPATION_CRIMINAL,
			Game::PED_THREAT_SEARCH_NO_THREATS,
			Game::PED_THREAT_REACTION_NO_REACTION,
			Game::PED_OBJECTIVE_FOLLOW_CAR_IN_CAR,
			9999,
			{ -2 },
			Game::PED_STATE_WALKING,
			Game::PED_STATE2_WALKING,
			static_cast<Game::PED_BIT_STATE>(1),
			static_cast<Game::PED_BIT_STATE2>(5),
			-1
		}, true, true},
		{ L"Cop", {
			Game::PED_GRAPHIC_TYPE_EMERG,
			Game::PED_REMAP_BLUE_POLICE,
			static_cast<Game::WEAPON_INDEX>(-1),
			50,
			Game::PED_OCCUPATION_POLICE_ON_FOOT,
			Game::PED_THREAT_SEARCH_LINE_OF_SIGHT,
			Game::PED_THREAT_REACTION_REACT_AS_EMERGENCY,
			Game::PED_OBJECTIVE_NO_OBJ,
			9999,
			{},
			Game::PED_STATE_WALKING,
			Game::PED_STATE2_WALKING,
			static_cast<Game::PED_BIT_STATE>(1),
			static_cast<Game::PED_BIT_STATE2>(3),
			-1
		}, true, true},
		{ L"Mugger", {
			Game::PED_GRAPHIC_TYPE_DUMMY,
			Game::PED_REMAP_MUGGER,
			static_cast<Game::WEAPON_INDEX>(-1),
			50,
			Game::PED_OCCUPATION_MUGGER,
			Game::PED_THREAT_SEARCH_AREA,
			Game::PED_THREAT_REACTION_RUN_AWAY,
			Game::PED_OBJECTIVE_NO_OBJ,
			0,
			{},
			Game::PED_STATE_WALKING,
			Game::PED_STATE2_WALKING,
			static_cast<Game::PED_BIT_STATE>(1),
			static_cast<Game::PED_BIT_STATE2>(3),
			-1
		}, true, true},
		{ L"Car thief", {
			Game::PED_GRAPHIC_TYPE_DUMMY,
			Game::PED_REMAP_GREEN_SHORTS_PED,
			static_cast<Game::WEAPON_INDEX>(-1),
			50,
			Game::PED_OCCUPATION_CARTHIEF,
			Game::PED_THREAT_SEARCH_AREA,
			Game::PED_THREAT_REACTION_RUN_AWAY,
			Game::PED_OBJECTIVE_NO_OBJ,
			0,
			{},
			Game::PED_STATE_WALKING,
			Game::PED_STATE2_WALKING,
			static_cast<Game::PED_BIT_STATE>(1),
			static_cast<Game::PED_BIT_STATE2>(3),
			-1
		}, true, true},
		{ L"SWAT", {
			Game::PED_GRAPHIC_TYPE_EMERG,
			Game::PED_REMAP_DEFAULT,
			Game::WEAPON_PISTOL,
			400,
			Game::PED_OCCUPATION_SWAT,
			Game::PED_THREAT_SEARCH_LINE_OF_SIGHT,
			Game::PED_THREAT_REACTION_REACT_AS_EMERGENCY,
			Game::PED_OBJECTIVE_NO_OBJ,
			9999,
			{},
			Game::PED_STATE_WALKING,
			Game::PED_STATE2_WALKING,
			static_cast<Game::PED_BIT_STATE>(1),
			static_cast<Game::PED_BIT_STATE2>(4),
			-1
		}, true, true},
		{ L"FBI", {
			Game::PED_GRAPHIC_TYPE_GANG,
			Game::PED_REMAP_ZAIBATSU,
			Game::WEAPON_SILENCED_S_UZI,
			250,
			Game::PED_OCCUPATION_FBI,
			Game::PED_THREAT_SEARCH_LINE_OF_SIGHT,
			Game::PED_THREAT_REACTION_REACT_AS_EMERGENCY,
			Game::PED_OBJECTIVE_NO_OBJ,
			9999,
			{},
			Game::PED_STATE_WALKING,
			Game::PED_STATE2_WALKING,
			static_cast<Game::PED_BIT_STATE>(1),
			static_cast<Game::PED_BIT_STATE2>(4),
			-1
		}, true, true},
		{ L"Bodyguard", {
			Game::PED_GRAPHIC_TYPE_GANG,
			Game::PED_REMAP_PLAYER,
			Game::WEAPON_SILENCED_S_UZI,
			100,
			Game::PED_OCCUPATION_SPECIAL_GROUP_MEMBER,
			Game::PED_THREAT_SEARCH_LINE_OF_SIGHT,
			Game::PED_THREAT_REACTION_REACT_AS_NORMAL,
			Game::PED_OBJECTIVE_NO_OBJ,
			9999,
			{},
			Game::PED_STATE_WALKING,
			Game::PED_STATE2_WALKING,
			static_cast<Game::PED_BIT_STATE>(1),
			static_cast<Game::PED_BIT_STATE2>(4),
			-3
		}, true, true },
		{ L"Anti-player guard", {
			Game::PED_GRAPHIC_TYPE_GANG,
			Game::PED_REMAP_YAKUZA,
			Game::WEAPON_S_UZI,
			100,
			Game::PED_OCCUPATION_GUARD,
			Game::PED_THREAT_SEARCH_AREA_PLAYER_ONLY,
			Game::PED_THREAT_REACTION_REACT_AS_NORMAL,
			Game::PED_OBJECTIVE_GUARD_SPOT,
			9999,
			{},
			Game::PED_STATE_WALKING,
			Game::PED_STATE2_WALKING,
			static_cast<Game::PED_BIT_STATE>(1),
			static_cast<Game::PED_BIT_STATE2>(3),
			-1
		}, true, true },
	};

	const std::unordered_map<std::wstring, const PedTemplate&> builtInPedTemplates = []() {
		std::unordered_map<std::wstring, const PedTemplate&> map;
		for (const auto& pedTemplate : builtInPedTemplatesArr) {
			map.emplace(pedTemplate.name, pedTemplate);
		}
		return map;
	}();

	const std::vector<std::wstring> builtInPedTemplateNames = []() {
		std::vector<std::wstring> names;
		for (const auto& pedTemplate : builtInPedTemplatesArr) {
			names.push_back(pedTemplate.name);
		}
		return names;
	}();
}