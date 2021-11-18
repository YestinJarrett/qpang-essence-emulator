#pragma once

#include "Character.h"

struct KenCharacter final : Character
{
	KenCharacter() : Character(KEN, 110, 5, true, 1.50, 1.00, 0.60, 0.40, 1.00, 2.00, 1.50, 1.80, 1.0, 1.0, 1.0, 1.0)
	{
	}
};