#pragma once

#include "Position.h"

struct CGHitInfo
{
	uint32_t sourcePlayerId;
	uint32_t targetPlayerId;

	Position sourcePlayerPosition;
	Position targetPlayerPosition;

	uint32_t entityId;

	uint32_t weaponId;
	uint8_t weaponType;

	uint8_t hitType;
	uint8_t hitLocation;

	uint8_t bodyPart;

	uint64_t rtt;

	uint32_t unknown03;
	uint8_t unknown17;
	uint32_t unknown18;
};