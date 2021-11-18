#pragma once

#include "Character.h"

struct KumaCharacter final : Character
{
	KumaCharacter() : Character(KUMA, 200, 8, true)
	{
	}
};