#pragma once

#include "Character.h"

struct HanaCharacter final : Character
{
	HanaCharacter() : Character(HANA, 100, 3, true, 1.50, 1.00, 0.60, 0.40, 1.00, 2.00, 1.50, 1.80, 1.00, 1.00, 1.00, 1.00)
	{
	}
};