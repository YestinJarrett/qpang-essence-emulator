#pragma once

#include <array>

#include "InventoryCard.h"
#include "InventoryCardWriter.h"
#include "packets/LobbyServerPacket.h"

class UpdateSkillCardSetResponse : public LobbyServerPacket
{
public:
	explicit UpdateSkillCardSetResponse(const std::array<InventoryCard, 3>& skillCards) : LobbyServerPacket(650)
	{
		for (const InventoryCard& skillCard : skillCards)
		{
			InventoryCardWriter::write(this, skillCard);
		}

		writeEmpty(22 * 43);
	}
};