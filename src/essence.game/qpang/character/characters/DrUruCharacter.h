#pragma once

#include "Character.h"

struct DrUruCharacter final : Character
{
	DrUruCharacter() : Character(DR_URU, 140, 4, true, 1.10, 0.90, 0.60, 0.50, 1.00, 2.00, 1.50, 1.80, 1.05, 1.00, 0.90, 1.05)
	{
	}
};