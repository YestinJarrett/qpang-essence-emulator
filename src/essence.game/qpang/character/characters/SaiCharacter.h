#pragma once

#include "Character.h"

struct SaiCharacter final : Character
{
	SaiCharacter() : Character(SAI, 130, 5, true, 1.20, 0.90, 0.60, 0.50, 1.00, 2.00, 1.50, 1.80, 1.05, 1.00, 0.90, 1.05)
	{
	}
};