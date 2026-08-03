#include "lists-utils.h"

const std::vector<Game::CAR_MODEL4>& Game::Utils::GetSpawnableCarModels() {
	static constexpr int32_t ids[] = {
		0,1,2,3,4,5,7,8,10,11,
		12,13,14,84,16,17,18,19,21,22,
		23,24,25,26,85,27,28,29,30,31,
		86,32,33,34,35,36,37,38,39,40,
		41,42,44,45,46,47,48,49,50,51,
		52,53,54,55,56,57,58,62,63,64,
		65,66,67,68,69,70,71,72,73,74,
		75,76,77,78,79,80,81,82,83
	};
	static const std::vector<CAR_MODEL4> models(
		reinterpret_cast<const CAR_MODEL4*>(std::begin(ids)),
		reinterpret_cast<const CAR_MODEL4*>(std::end(ids))
	);
	return models;
}

const std::vector<short>& Game::Utils::GetAvailableCarRemaps() {
	static constexpr short ids[] = {
		-1,0,1,2,3,4,5,6,7,8,9,
		10,11,12,13,14,15,16,17,18,19,
		20,21,22,23,24,25,26,27,28,29,
		30,31,32,33,34,35,36
	};
	static const std::vector<short> remaps(std::begin(ids), std::end(ids));
	return remaps;
}

const std::vector<std::tuple<short, Game::PALETTE_BASE>>& Game::Utils::GetAvailableCarRemapsWithPalette() {
	static std::vector<std::tuple<short, PALETTE_BASE>> remaps;
	if (!remaps.empty()) return remaps;

	remaps.emplace_back(0, PALETTE_BASE::PALETTE_BASE_SPRITE);
	for (short i = 0; i <= 36; ++i) {
		remaps.emplace_back(i, PALETTE_BASE::PALETTE_BASE_CAR_REMAP);
	}
	return remaps;
}

const std::vector<Game::WEAPON_INDEX>& Game::Utils::GetAvailableWeapons() {
	static constexpr int32_t ids[] = {
		0,1,2,3,4,5,7,8,9,10,
		11,12,13,14,15,16,17,18,19,20,
		21,22,23,24,25,26,27
	};
	static const std::vector<WEAPON_INDEX> weapons(
		reinterpret_cast<const WEAPON_INDEX*>(std::begin(ids)),
		reinterpret_cast<const WEAPON_INDEX*>(std::end(ids))
	);
	return weapons;
}

const std::vector<Game::WEAPON_INDEX> Game::Utils::GetAvailableWeaponsInclNoWeapon()
{
	static constexpr int32_t ids[] = {
		   -1,0,1,2,3,4,5,7,8,9,10,
		   11,12,13,14,15,16,17,18,19,20,
		   21,22,23,24,25,26,27
	};
	static const std::vector<WEAPON_INDEX> weapons(
		reinterpret_cast<const WEAPON_INDEX*>(std::begin(ids)),
		reinterpret_cast<const WEAPON_INDEX*>(std::end(ids))
	);
	return weapons;
}

const std::vector<Game::EXPLOSION_SIZE>& Game::Utils::GetAvailableExplosionSizes() {
	static const std::vector<EXPLOSION_SIZE> explosionSizes({
		EXPLOSION_SIZE_NONE,
		EXPLOSION_SIZE_SMALL,
		EXPLOSION_SIZE_MEDIUM,
		EXPLOSION_SIZE_LARGE
	});
	return explosionSizes;
}

const std::vector<Game::PED_REMAP>& Game::Utils::GetAvailablePedRemaps()
{
	static constexpr unsigned char ids[] = {
		-1,0,1,2,3,4,5,6,7,8,9,
		10,11,12,13,14,15,16,17,18,19,
		20,21,22,23,24,25,26,27,28,29,
		30,31,32,33,34,35,36,37,38,39,
		40,41,42,43,44,45,46,47,48,49,
		50,51,52
	};
	static const std::vector<PED_REMAP> remaps(
		reinterpret_cast<const PED_REMAP*>(std::begin(ids)),
		reinterpret_cast<const PED_REMAP*>(std::end(ids))
	);
	return remaps;
}

const std::vector<Game::PED_GRAPHIC_TYPE>& Game::Utils::GetAvailablePedGraphicTypes()
{
	static const std::vector<PED_GRAPHIC_TYPE> graphicTypes({
		PED_GRAPHIC_TYPE_DUMMY,
		PED_GRAPHIC_TYPE_GANG,
		PED_GRAPHIC_TYPE_EMERG
	});
	return graphicTypes;
}

const std::vector<Game::POWERUP_TYPE>& Game::Utils::GetAvailablePowerupTypes() {
	static constexpr int32_t ids[] = {
		0,1,2,3,4,5,6,7,8,9,
		10,11,12,13,14,15,16
	};
	static const std::vector<POWERUP_TYPE> powerupTypes(
		reinterpret_cast<const POWERUP_TYPE*>(std::begin(ids)),
		reinterpret_cast<const POWERUP_TYPE*>(std::end(ids))
	);
	return powerupTypes;
}

const std::vector<short>& Game::Utils::GetAvailableCopValues()
{
	static const std::vector<short> copValues({
		0,
		600,
		1600,
		3000,
		5000,
		8000,
		12000
	});
	return copValues;
}

const std::vector<Game::PED_OBJECTIVE>& Game::Utils::GetAvailablePedObjectives()
{
	static constexpr int32_t ids[] = {
		0,1,2,3,4,5,6,7,8,9,
		10,11,12,13,14,15,16,17,18,19,
		20,21,22,23,24,25,26,27,28,29,
		30,31,32,33,34,35,36,37,38,39,
		40,41,42,43,44,45,46,47,48,49,
		50,51,52,53,54,55,56,57,58,59,
		60,61
	};

	static const std::vector<PED_OBJECTIVE> objectives(
		reinterpret_cast<const PED_OBJECTIVE*>(std::begin(ids)),
		reinterpret_cast<const PED_OBJECTIVE*>(std::end(ids))
	);

	return objectives;
}

const std::vector<Game::PED_OCCUPATION>& Game::Utils::GetAvailablePedOccupations()
{
	static constexpr int32_t ids[] = {
		0,1,2,3,4,5,6,7,8,9,
		10,11,12,13,14,15,16,17,18,19,
		20,21,22,23,24,25,26,27,28,29,
		30,31,32,33,34,35,36,37,38,39,
		40,41,42,43,44,45,46,47,48,49,
		50,51
	};

	static const std::vector<PED_OCCUPATION> occupations(
		reinterpret_cast<const PED_OCCUPATION*>(std::begin(ids)),
		reinterpret_cast<const PED_OCCUPATION*>(std::end(ids))
	);

	return occupations;
}

const std::vector<Game::PED_STATE>& Game::Utils::GetAvailablePedStates()
{
	static constexpr int32_t ids[] = {
		0,1,2,3,4,5,6,7,8,9,
		10,11,12,13,14,15,16,17,18,19,
		20,21,22
	};

	static const std::vector<PED_STATE> states(
		reinterpret_cast<const PED_STATE*>(std::begin(ids)),
		reinterpret_cast<const PED_STATE*>(std::end(ids))
	);

	return states;
}

const std::vector<Game::PED_STATE2>& Game::Utils::GetAvailablePedStates2()
{
	static constexpr int32_t ids[] = {
		0,1,2,3,4,5,6,7,8,9,
		10,11,12,13,14,15,16,17,18,19,
		20,21,22,23,24,25,26
	};

	static const std::vector<PED_STATE2> states(
		reinterpret_cast<const PED_STATE2*>(std::begin(ids)),
		reinterpret_cast<const PED_STATE2*>(std::end(ids))
	);

	return states;
}

const std::vector<Game::PED_THREAT_REACTION>& Game::Utils::GetAvailablePedThreatReactions()
{
	static const std::vector<PED_THREAT_REACTION> threatReactions = {
		PED_THREAT_REACTION_NO_REACTION,
		PED_THREAT_REACTION_REACT_AS_EMERGENCY,
		PED_THREAT_REACTION_REACT_AS_NORMAL,
		PED_THREAT_REACTION_RUN_AWAY
	};

	return threatReactions;
}

const std::vector<Game::PED_THREAT_SEARCH>& Game::Utils::GetAvailablePedThreatSearches()
{
	static const std::vector<PED_THREAT_SEARCH> threatSearches = {
		PED_THREAT_SEARCH_NO_THREATS,
		PED_THREAT_SEARCH_LINE_OF_SIGHT,
		PED_THREAT_SEARCH_AREA,
		PED_THREAT_SEARCH_AREA_PLAYER_THREAT_ONLY,
		PED_THREAT_SEARCH_LINE_OF_SIGHT_PLAYER_THREAT_ONLY,
		PED_THREAT_SEARCH_AREA_PLAYER_ONLY,
		PED_THREAT_SEARCH_LINE_OF_SIGHT_PLAYER_ONLY
	};

	return threatSearches;
}
