#pragma once

#include "Character.h"

struct SaiCharacter final : Character
{
	SaiCharacter() : Character(SAI, 130, 5, true)
	{
	}
};