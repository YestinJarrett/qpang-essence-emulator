#pragma once

#include "qpang/player/Player.h"
#include "qpang/room/RoomPlayer.h"
#include "qpang/room/Room.h"
#include "qpang/room/session/RoomSession.h"
#include <qpang/Game.h>

#include "qpang/chat/command/Command.h"

#include <string.h>
#include <qpang/room/tnl/net_events/server/gc_game_state.hpp>
#include <qpang/room/tnl/net_events/server/gc_card.hpp>

class FakeDeathCommand : public Command
{
public:
	FakeDeathCommand() : Command(1)
	{

	}

	void handle(std::shared_ptr<Player> player, const std::vector<std::u16string>& args)
	{
		const auto roomPlayer = player->getRoomPlayer();

		if (roomPlayer == nullptr)
		{
			player->broadcast(u"You need to be in a game in order to execute this command.");

			return;
		}

		const auto roomSessionPlayer = roomPlayer->getRoomSessionPlayer();

		if (roomSessionPlayer == nullptr)
		{
			player->broadcast(u"You need to be in a game in order to execute this command.");

			return;
		}

		const auto roomSession = roomSessionPlayer->getRoomSession();

		if (roomSession == nullptr)
		{
			player->broadcast(u"You need to be in a game in order to execute this command.");

			return;
		}

		if (roomSessionPlayer->isDead() || roomSessionPlayer->isInvincible())
		{
			player->broadcast(u"You may not use this command right now.");

			return;
		}

		if (roomSession->getGameMode()->isPublicEnemyMode())
		{
			player->broadcast(u"You may not use this command in public enemy mode.");

			return;
		}

		const auto uid = player->getId();
		const auto targetUid = uid;

		roomSession->relayPlaying<GCCard>(uid, targetUid, cmd, cardType, itemId, seqId);

		player->broadcast(u"You have used fake death.");
	}
private:
	const int cmd = 4;
	const int cardType = 7;

	const int itemId = 1409286146;
	const int seqId = itemId;
};