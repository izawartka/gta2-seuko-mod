#include "categorized-objects-utils.h"

namespace ModMenuModule::Utils::CategorizedObjects {
	static const std::vector<ObjectCategory> categories = {
		ObjectCategory::Basic,
		ObjectCategory::WeaponCollectibles,
		ObjectCategory::CarWeaponCollectibles,
		ObjectCategory::Powerups,
		ObjectCategory::Skidmarks,
		ObjectCategory::Projectiles,
		ObjectCategory::Phones,
		ObjectCategory::Other,
		ObjectCategory::Invisible,
	};

	static const std::vector<ObjectDef> objectDefs = {
		{ L"Bin lid", ObjectCategory::Basic, {{ ObjectVariant::Default, Game::OBJECT_TYPE_BIN_LID }, { ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_BIN_LID }} },
		{ L"Bollard", ObjectCategory::Basic, {{ ObjectVariant::Default, Game::OBJECT_TYPE_BOLLARD }} },
		{ L"Cone", ObjectCategory::Basic, {{ ObjectVariant::Default, Game::OBJECT_TYPE_CONE }, { ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_CONE }, { ObjectVariant::Dead, Game::OBJECT_TYPE_DEAD_CONE }} },
		{ L"Boxes", ObjectCategory::Basic, {{ ObjectVariant::Default, Game::OBJECT_TYPE_BOXES }, { ObjectVariant::Dead, Game::OBJECT_TYPE_DEAD_BOXES }} },
		{ L"Blaster", ObjectCategory::Basic, {{ ObjectVariant::Default, Game::OBJECT_TYPE_BLASTER }, { ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_BLASTER }, { ObjectVariant::Dead, Game::OBJECT_TYPE_DEAD_BLASTER }} },
		{ L"Rubbish", ObjectCategory::Basic, {{ ObjectVariant::Default, Game::OBJECT_TYPE_RUBBISH }, { ObjectVariant::Animating, Game::OBJECT_TYPE_ANIMATING_RUBBISH }, { ObjectVariant::Dead, Game::OBJECT_TYPE_DEAD_RUBBISH }} },
		{ L"Bin", ObjectCategory::Basic, {{ ObjectVariant::Default, Game::OBJECT_TYPE_BIN }, { ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_BIN }} },
		{ L"Oil", ObjectCategory::Basic, {{ ObjectVariant::Default, Game::OBJECT_TYPE_OIL }, { ObjectVariant::Animating, Game::OBJECT_TYPE_ANIMATING_OIL }} },
		{ L"Mine", ObjectCategory::Basic, {{ ObjectVariant::Default, Game::OBJECT_TYPE_MINE }, { ObjectVariant::Dead, Game::OBJECT_TYPE_DEAD_MINE }} },
		{ L"Bush", ObjectCategory::Basic, {{ ObjectVariant::Default, Game::OBJECT_TYPE_BUSH }, { ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_BUSH }, { ObjectVariant::Dead, Game::OBJECT_TYPE_DEAD_BUSH }} },
		{ L"Crate", ObjectCategory::Basic, {{ ObjectVariant::Default, Game::OBJECT_TYPE_CRATE }, { ObjectVariant::Animating, Game::OBJECT_TYPE_ANIMATING_CRATE }, { ObjectVariant::Dead, Game::OBJECT_TYPE_DEAD_CRATE }} },
		{ L"Footy", ObjectCategory::Basic, {{ ObjectVariant::Default, Game::OBJECT_TYPE_FOOTY }, { ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_FOOTY }} },
		{ L"Hardbox", ObjectCategory::Basic, {{ ObjectVariant::Default, Game::OBJECT_TYPE_HARDBOX }, { ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_HARDBOX }, { ObjectVariant::Dead, Game::OBJECT_TYPE_DEAD_HARDBOX }} },
		{ L"Newsdis", ObjectCategory::Basic, {{ ObjectVariant::Default, Game::OBJECT_TYPE_NEWSDIS }, { ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_NEWSDIS }, { ObjectVariant::Dead, Game::OBJECT_TYPE_DEAD_NEWSDIS }} },
		{ L"Oildrum", ObjectCategory::Basic, {{ ObjectVariant::Default, Game::OBJECT_TYPE_OILDRUM }, { ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_OILDRUM }, { ObjectVariant::Dead, Game::OBJECT_TYPE_DEAD_OILDRUM }} },
		{ L"Tyre", ObjectCategory::Basic, {{ ObjectVariant::Default, Game::OBJECT_TYPE_TYRE }, { ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_TYRE }} },
		{ L"Hydrant lid", ObjectCategory::Basic, {{ ObjectVariant::Default, Game::OBJECT_TYPE_HYDRANT_LID }, { ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_HYDRANT_LID }} },
		{ L"Hydrant", ObjectCategory::Basic, {{ ObjectVariant::Default, Game::OBJECT_TYPE_HYDRANT }} },
		{ L"Hydrant unlid", ObjectCategory::Basic, {{ ObjectVariant::Default, Game::OBJECT_TYPE_HYDRANT_UNLID }} },
		{ L"Roadblock", ObjectCategory::Basic, {{ ObjectVariant::Default, Game::OBJECT_TYPE_ROADBLOCK }, { ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_ROADBLOCK }, { ObjectVariant::Dead, Game::OBJECT_TYPE_DEAD_ROADBLOCK }} },
		{ L"Bench", ObjectCategory::Basic, {{ ObjectVariant::Default, Game::OBJECT_TYPE_BENCH }, { ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_BENCH }, { ObjectVariant::Dead, Game::OBJECT_TYPE_DEAD_BENCH }} },
		{ L"Package", ObjectCategory::Basic, {{ ObjectVariant::Default, Game::OBJECT_TYPE_PACKAGE }, { ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_PACKAGE }, { ObjectVariant::Dead, Game::OBJECT_TYPE_DEAD_PACKAGE }} },
		{ L"Unknown obj", ObjectCategory::Basic, {{ ObjectVariant::Default, Game::OBJECT_TYPE_UNKNOWN_OBJ }} },
		{ L"Explode medium", ObjectCategory::Basic, {{ ObjectVariant::Default, Game::OBJECT_TYPE_EXPLODE_MEDIUM }} },
		{ L"Powergen", ObjectCategory::Basic, {{ ObjectVariant::Default, Game::OBJECT_TYPE_POWERGEN }, { ObjectVariant::Dead, Game::OBJECT_TYPE_POWERGEN_DEAD }} },

		{ L"Pistol", ObjectCategory::WeaponCollectibles, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_00_PISTOL }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_00_PISTOL }} },
		{ L"S-Uzi", ObjectCategory::WeaponCollectibles, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_01_S_UZI }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_01_S_UZI }} },
		{ L"Rocket Launcher", ObjectCategory::WeaponCollectibles, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_02_ROCKET_LAUNCHER }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_02_ROCKET_LAUNCHER }} },
		{ L"ElectroGun", ObjectCategory::WeaponCollectibles, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_03_ELECTROGUN }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_03_ELECTROGUN }} },
		{ L"Molotov Coctail", ObjectCategory::WeaponCollectibles, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_04_MOLOTOV_COCTAIL }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_04_MOLOTOV_COCTAIL }} },
		{ L"Grenade", ObjectCategory::WeaponCollectibles, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_05_GRENADE }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_05_GRENADE }} },
		{ L"Shotgun", ObjectCategory::WeaponCollectibles, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_06_SHOTGUN }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_06_SHOTGUN }} },
		{ L"ElectroBaton", ObjectCategory::WeaponCollectibles, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_07_ELECTROBATON }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_07_ELECTROBATON }} },
		{ L"Flamethrower", ObjectCategory::WeaponCollectibles, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_08_FLAMETHROWER }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_08_FLAMETHROWER }} },
		{ L"Silenced S-Uzi", ObjectCategory::WeaponCollectibles, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_09_SILENCED_S_UZI }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_09_SILENCED_S_UZI }} },
		{ L"Dual Pistol", ObjectCategory::WeaponCollectibles, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_10_DUAL_PISTOL }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_10_DUAL_PISTOL }} },
		{ L"Letter L", ObjectCategory::WeaponCollectibles, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_11_LETTER_L }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_11_LETTER_L }} },
		{ L"Letter M", ObjectCategory::WeaponCollectibles, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_12_LETTER_M }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_12_LETTER_M }} },
		{ L"Letter N", ObjectCategory::WeaponCollectibles, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_13_LETTER_N }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_13_LETTER_N }} },
		{ L"Letter O", ObjectCategory::WeaponCollectibles, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_14_LETTER_O }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_14_LETTER_O }} },

		{ L"Vehicle bomb", ObjectCategory::CarWeaponCollectibles, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_15_VEHICLE_BOMB }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_15_VEHICLE_BOMB }} },
		{ L"Vehicle Oil Slick", ObjectCategory::CarWeaponCollectibles, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_16_VEHICLE_OIL_SLICK }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_16_VEHICLE_OIL_SLICK }} },
		{ L"Vehicle Mine", ObjectCategory::CarWeaponCollectibles, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_17_VEHICLE_MINE }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_17_VEHICLE_MINE }} },
		{ L"Vehicle Machine Gun", ObjectCategory::CarWeaponCollectibles, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_18_VEHICLE_MACHINE_GUN }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_18_VEHICLE_MACHINE_GUN }} },
		{ L"Tank Cannon", ObjectCategory::CarWeaponCollectibles, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_19_TANK_CANNON }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_19_TANK_CANNON }} },
		{ L"Water Cannon", ObjectCategory::CarWeaponCollectibles, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_20_WATER_CANNON }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_20_WATER_CANNON }} },
		{ L"Vehicle Flamethrower", ObjectCategory::CarWeaponCollectibles, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_21_VEHICLE_FLAMETHROWER }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_21_VEHICLE_FLAMETHROWER }} },
		{ L"Jeep Turret", ObjectCategory::CarWeaponCollectibles, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_22_JEEP_TURRET }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_22_JEEP_TURRET }} },
		{ L"Instant Vehicle Bomb", ObjectCategory::CarWeaponCollectibles, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_23_INSTANT_VEHICLE_BOMB }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_23_INSTANT_VEHICLE_BOMB }} },
		{ L"Letter J", ObjectCategory::CarWeaponCollectibles, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_24_LETTER_J }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_24_LETTER_J }} },
		{ L"Letter K", ObjectCategory::CarWeaponCollectibles, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_25_LETTER_K }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_25_LETTER_K }} },
		{ L"Letter L", ObjectCategory::CarWeaponCollectibles, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_26_LETTER_L }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_26_LETTER_L }} },
		{ L"Letter M", ObjectCategory::CarWeaponCollectibles, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_27_LETTER_M }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_27_LETTER_M }} },

		{ L"Multiplier +1", ObjectCategory::Powerups, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_28_MULTIPLIER_PLUS_1 }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_28_MULTIPLIER_PLUS_1 }} },
		{ L"Life +1", ObjectCategory::Powerups, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_29_LIFE_PLUS_1 }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_29_LIFE_PLUS_1 }} },
		{ L"Health", ObjectCategory::Powerups, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_30_HEALTH }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_30_HEALTH }} },
		{ L"Armor", ObjectCategory::Powerups, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_31_ARMOR }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_31_ARMOR }} },
		{ L"Get Outta Jail Free Card", ObjectCategory::Powerups, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_32_GET_OUTTA_JAIL_FREE_CARD }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_32_GET_OUTTA_JAIL_FREE_CARD }} },
		{ L"Cop Bribe", ObjectCategory::Powerups, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_33_COP_BRIBE }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_33_COP_BRIBE }} },
		{ L"Invulnerability", ObjectCategory::Powerups, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_34_INVULNERABILITY }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_34_INVULNERABILITY }} },
		{ L"Double Damage", ObjectCategory::Powerups, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_35_DOUBLE_DAMAGE }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_35_DOUBLE_DAMAGE }} },
		{ L"Fast Reload", ObjectCategory::Powerups, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_36_FAST_RELOAD }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_36_FAST_RELOAD }} },
		{ L"ElectroFingers", ObjectCategory::Powerups, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_37_ELECTROFINGERS }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_37_ELECTROFINGERS }} },
		{ L"Respect!", ObjectCategory::Powerups, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_38_RESPECT }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_38_RESPECT }} },
		{ L"Invisibility", ObjectCategory::Powerups, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_39_INVISIBILITY }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_39_INVISIBILITY }} },
		{ L"Instant Gang", ObjectCategory::Powerups, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_40_INSTANT_GANG }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_40_INSTANT_GANG }} },
		{ L"Letter N", ObjectCategory::Powerups, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_41_LETTER_N }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_41_LETTER_N }} },
		{ L"Letter O", ObjectCategory::Powerups, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_42_LETTER_O }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_42_LETTER_O }} },
		{ L"Letter P", ObjectCategory::Powerups, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_43_LETTER_P }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_43_LETTER_P }} },
		{ L"Letter Q", ObjectCategory::Powerups, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOVING_COLLECT_44_LETTER_Q }, { ObjectVariant::Default, Game::OBJECT_TYPE_COLLECT_44_LETTER_Q }} },
		{ L"Bonus Token", ObjectCategory::Powerups, {{ ObjectVariant::Default, Game::OBJECT_TYPE_BONUS_TOKEN }} },
		{ L"Kill Frenzy", ObjectCategory::Powerups, {{ ObjectVariant::Default, Game::OBJECT_TYPE_KILL_FRENZY }} },
		{ L"Remote", ObjectCategory::Powerups, {{ ObjectVariant::Default, Game::OBJECT_TYPE_REMOTE }} },

		{ L"Brown skid", ObjectCategory::Skidmarks, {{ ObjectVariant::Small, Game::OBJECT_TYPE_SMALL_BROWN_SKID }, { ObjectVariant::Medium, Game::OBJECT_TYPE_MEDIUM_BROWN_SKID }, { ObjectVariant::Big, Game::OBJECT_TYPE_BIG_BROWN_SKID }, { ObjectVariant::Huge, Game::OBJECT_TYPE_HUGE_BROWN_SKID }} },
		{ L"Grey skid", ObjectCategory::Skidmarks, {{ ObjectVariant::Small, Game::OBJECT_TYPE_SMALL_GREY_SKID }, { ObjectVariant::Medium, Game::OBJECT_TYPE_MEDIUM_GREY_SKID }, { ObjectVariant::Big, Game::OBJECT_TYPE_BIG_GREY_SKID }, { ObjectVariant::Huge, Game::OBJECT_TYPE_HUGE_GREY_SKID }} },
		{ L"Red skid", ObjectCategory::Skidmarks, {{ ObjectVariant::Small, Game::OBJECT_TYPE_SMALL_RED_SKID }, { ObjectVariant::Medium, Game::OBJECT_TYPE_MEDIUM_RED_SKID }, { ObjectVariant::Big, Game::OBJECT_TYPE_BIG_RED_SKID }, { ObjectVariant::Huge, Game::OBJECT_TYPE_HUGE_RED_SKID }} },
		{ L"White skid", ObjectCategory::Skidmarks, {{ ObjectVariant::Small, Game::OBJECT_TYPE_SMALL_WHITE_SKID }, { ObjectVariant::Medium, Game::OBJECT_TYPE_MEDIUM_WHITE_SKID }, { ObjectVariant::Big, Game::OBJECT_TYPE_BIG_WHITE_SKID }, { ObjectVariant::Huge, Game::OBJECT_TYPE_HUGE_WHITE_SKID }} },

		{ L"Rocket", ObjectCategory::Projectiles, {{ ObjectVariant::Default, Game::OBJECT_TYPE_ROCKET }} },
		{ L"Molotov", ObjectCategory::Projectiles, {{ ObjectVariant::Moving, Game::OBJECT_TYPE_MOLOTOV_MOVING }} },
		{ L"Grenade", ObjectCategory::Projectiles, {{ ObjectVariant::Default, Game::OBJECT_TYPE_GRENADE }} },
		{ L"Shot", ObjectCategory::Projectiles, {{ ObjectVariant::Default, Game::OBJECT_TYPE_SHOT }} },
		{ L"Flaming bullet", ObjectCategory::Projectiles, {{ ObjectVariant::Default, Game::OBJECT_TYPE_FLAMING_BULLET }} },
		{ L"Water bullet", ObjectCategory::Projectiles, {{ ObjectVariant::Default, Game::OBJECT_TYPE_WATER_BULLET }} },
		{ L"Bullet", ObjectCategory::Projectiles, {{ ObjectVariant::Default, Game::OBJECT_TYPE_BULLET }} },
		{ L"Pistol bullet", ObjectCategory::Projectiles, {{ ObjectVariant::Default, Game::OBJECT_TYPE_PISTOL_BULLET }} },
		{ L"Tasser bullet", ObjectCategory::Projectiles, {{ ObjectVariant::Default, Game::OBJECT_TYPE_TASSER_BULLET }} },

		{ L"Phone", ObjectCategory::Phones, {{ ObjectVariant::Default, Game::OBJECT_TYPE_PHONE }, { ObjectVariant::Ringing, Game::OBJECT_TYPE_PHONE_RINGING }, { ObjectVariant::Dead, Game::OBJECT_TYPE_PHONE_DEAD }} },
		{ L"Red phone", ObjectCategory::Phones, {{ ObjectVariant::Default, Game::OBJECT_TYPE_RED_PHONE }, { ObjectVariant::Ringing, Game::OBJECT_TYPE_RED_PHONE_RINGING }} },
		{ L"Yellow phone", ObjectCategory::Phones, {{ ObjectVariant::Default, Game::OBJECT_TYPE_YELLOW_PHONE }, { ObjectVariant::Ringing, Game::OBJECT_TYPE_YELLOW_PHONE_RINGING }} },
		{ L"Green phone", ObjectCategory::Phones, {{ ObjectVariant::Default, Game::OBJECT_TYPE_GREEN_PHONE }, { ObjectVariant::Ringing, Game::OBJECT_TYPE_GREEN_PHONE_RINGING }} },

		{ L"Small arrow", ObjectCategory::Other, {{ ObjectVariant::Default, Game::OBJECT_TYPE_SMALL_ARROW }} },
		{ L"Blood spark", ObjectCategory::Other, {{ ObjectVariant::Default, Game::OBJECT_TYPE_BLOOD_SPARK }} },
		{ L"Firejet", ObjectCategory::Other, {{ ObjectVariant::Default, Game::OBJECT_TYPE_FIREJET }} },
		{ L"Tanktop", ObjectCategory::Other, {{ ObjectVariant::Default, Game::OBJECT_TYPE_TANKTOP }} },
		{ L"Antenna", ObjectCategory::Other, {{ ObjectVariant::Default, Game::OBJECT_TYPE_ANTENNA }} },
		{ L"Briefcase", ObjectCategory::Other, {{ ObjectVariant::Default, Game::OBJECT_TYPE_BRIEFCASE }} },
		{ L"Fire", ObjectCategory::Other, {{ ObjectVariant::Default, Game::OBJECT_TYPE_FIRE }} },
		{ L"Bomb", ObjectCategory::Other, {{ ObjectVariant::Default, Game::OBJECT_TYPE_BOMB }} },
		{ L"Traffic light", ObjectCategory::Other, {{ ObjectVariant::Default, Game::OBJECT_TYPE_TRAFFIC_LIGHT }} },
		{ L"Red footprints", ObjectCategory::Other, {{ ObjectVariant::Default, Game::OBJECT_TYPE_RED_FOOTPRINTS }} },
		{ L"Blood", ObjectCategory::Other, {{ ObjectVariant::Default, Game::OBJECT_TYPE_BLOOD }} },
		{ L"Spark", ObjectCategory::Other, {{ ObjectVariant::Default, Game::OBJECT_TYPE_SPARK }} },
		{ L"Genlite", ObjectCategory::Other, {{ ObjectVariant::Default, Game::OBJECT_TYPE_GENLITE }} },

		{ L"Tower", ObjectCategory::Invisible, {{ ObjectVariant::Default, Game::OBJECT_TYPE_TOWER }} },
		{ L"Particle system", ObjectCategory::Invisible, {{ ObjectVariant::Default, Game::OBJECT_TYPE_PARTICLE_SYSTEM }} },
		{ L"Car crossing", ObjectCategory::Invisible, {{ ObjectVariant::Default, Game::OBJECT_TYPE_CAR_CROSSING }} },
		{ L"Bus stop marker", ObjectCategory::Invisible, {{ ObjectVariant::Default, Game::OBJECT_TYPE_BUS_STOP_MARKER }} },
		{ L"Car shop", ObjectCategory::Invisible, {{ ObjectVariant::Default, Game::OBJECT_TYPE_CAR_SHOP }} },
		{ L"Busy car shop", ObjectCategory::Invisible, {{ ObjectVariant::Default, Game::OBJECT_TYPE_BUSY_CAR_SHOP }} },
		{ L"Car bomb", ObjectCategory::Invisible, {{ ObjectVariant::Default, Game::OBJECT_TYPE_CAR_BOMB }} },
		{ L"Save point", ObjectCategory::Invisible, {{ ObjectVariant::Default, Game::OBJECT_TYPE_SAVE_POINT }} },
		{ L"Sound instant", ObjectCategory::Invisible, {{ ObjectVariant::Default, Game::OBJECT_TYPE_SOUND_INSTANT }} },
		{ L"Invisible target", ObjectCategory::Invisible, {{ ObjectVariant::Default, Game::OBJECT_TYPE_INVISIBLE_TARGET }} },
		{ L"Invisible destructible", ObjectCategory::Invisible, {{ ObjectVariant::Default, Game::OBJECT_TYPE_INVISIBLE_DESTRUCTIBLE }} },
		{ L"Invisible dead", ObjectCategory::Invisible, {{ ObjectVariant::Default, Game::OBJECT_TYPE_INVISIBLE_DEAD }} },
		{ L"Crossing", ObjectCategory::Invisible, {{ ObjectVariant::Default, Game::OBJECT_TYPE_CROSSING }} },
		{ L"Tunnel blocker", ObjectCategory::Invisible, {{ ObjectVariant::Default, Game::OBJECT_TYPE_TUNNEL_BLOCKER }} },
	};

	static const std::unordered_map<Game::OBJECT_TYPE, std::pair<const ObjectDef*, ObjectVariant>> objectTypeToDefMap = []() {
		std::unordered_map<Game::OBJECT_TYPE, std::pair<const ObjectDef*, ObjectVariant>> map;
		for (const auto& def : objectDefs) {
			for (const auto& variantPair : def.variants) {
				map[variantPair.second] = { &def, variantPair.first };
			}
		}
		return map;
	}();

	static const std::unordered_map<ObjectCategory, std::vector<const ObjectDef*>> objectDefsByCategoryMap = []() {
		std::unordered_map<ObjectCategory, std::vector<const ObjectDef*>> map;
		for (const auto& def : objectDefs) {
			map[def.category].push_back(&def);
		}
		return map;
	}();
}
