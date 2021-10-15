#pragma once

#include <cstdint>

enum ItemID : uint32_t
{
	EXP_MAKER_25 = 1174405140,
	DON_MAKER_25 = 1174405141,
	EXP_MAKER_50 = 1174405157,
	DON_MAKER_50 = 1174405182,
	SLACKER_BLESSING = 1174405142,
	QUICK_REVIVE = 1174405146,
	NAME_CHANGER = 1174405145,
	KD_CLEANER = 1174405143,
	WL_CLEANER = 1174405144,
	CRANE_MACHINE_ELITE = 1174405151,
	GOLD_WRENCH = 1174405221,
	FRIEND_MAKER = 1174405150,
	ESSENCE_CHIP = 1174405152,
	PVE_COIN_BOOSTER = 1174405220,
	PVE_PANTH_CHEST = 1174405162,
	PVE_PANTH_KEY = 1174405165,
	PVE_PANTH_BLESSING = 1174405164,

	SKILL_NONE = 0,
	SKILL_HAWK_EYE = 1258356737,
	SKILL_RAPID_FIRE = 1258356738,
	SKILL_RAGE = 1258356741,
	SKILL_SHACKLES = 1258356742,
	SKILL_CAMO = 1258356753,
	SKILL_CHAOS = 1258356756,
	SKILL_POCKET_SIZE = 1258356764,
	SKILL_VITAL = 1258356776,
	SKILL_TRANSE = 1258356777,
	SKILL_GIANT_BEAST = 1258356778,
	SKILL_LAST_WEAPON = 1258356779
};

static bool isEquippableFunction(uint32_t id)
{
	return
		id == ItemID::EXP_MAKER_25 ||
		id == ItemID::EXP_MAKER_50 ||
		id == ItemID::DON_MAKER_25 ||
		id == ItemID::DON_MAKER_50 ||
		id == ItemID::QUICK_REVIVE ||
		id == ItemID::PVE_COIN_BOOSTER ||
		id == ItemID::PVE_PANTH_BLESSING;
}