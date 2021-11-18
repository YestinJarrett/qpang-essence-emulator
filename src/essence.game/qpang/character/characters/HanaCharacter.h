#pragma once

#include "Character.h"

struct HanaCharacter final : Character
{
	HanaCharacter() : Character(HANA, 100, 3, true)
	{
	}
};