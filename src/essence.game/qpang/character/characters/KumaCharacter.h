#pragma once

#include "Character.h"

struct KumaCharacter final : Character
{
	KumaCharacter() : Character(KUMA, 200, 8, true, 1.50, 1.00, 0.50, 0.50, 1.00, 2.00, 1.50, 1.80, 1.10, 1.00, 0.90, 1.00)
	{
	}
};