#pragma once

#include <vector>

#include "packets/LobbyServerPacket.h"
#include "packets/writers/RoomWriter.h"

#include "qpang/room/Room.h"

class RoomList : public LobbyServerPacket
{
public:
	RoomList(const std::vector<Room::Ptr>& rooms) : LobbyServerPacket(759)
	{
		auto size = static_cast<uint16_t>(rooms.size());

		writeShort(size);
		writeShort(size);
		writeShort(size);

		for (const auto& room : rooms)
			RoomWriter::write(this, room);
	}
};