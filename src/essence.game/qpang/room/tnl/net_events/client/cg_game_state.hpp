#ifndef CG_GAME_STATE_H
#define CG_GAME_STATE_H

#include "GameNetEvent.h"

#include "qpang/room/tnl/net_events/server/gc_game_state.hpp"
#include "qpang/room/session/RoomSession.h"

class CGGameState : public GameNetEvent
{
	typedef NetEvent Parent;
public:

	enum State : U32 {
		GAME_WAITING_PLAYERS_SPECTATE_UNK = 1,
		GAME_FORCE_LEAVE_STO = 2,
		GAME_WAITING_PLAYERS = 3,
		SEND_GAME_STATE_PLAY = 4,
		DISCONNECT_P2P = 7,
		REMOVE_INVINCIBILITY = 8,
		SYNC_TIME = 11,
		GAME_START = 12,
		GAME_START_PVE = GAME_START,
		GAME_STATE_P2P_FAIL = 14,
		LEAVE_GAME = 15,
		UPDATE_HEALTH = 16,
		KILLFEED_ADD = 17,
		PLAYER_STATE_HACK = 22,
		GAME_OVER = 23,
		INV_IN = 24,
		INV_OUT = 25,
		KILLFEED_ADD_HEAD = 28,
		START_RESPAWN_TIMER = 29,
	};
	CGGameState() : GameNetEvent{ CG_GAME_STATE, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirClientToServer } {};

	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream)
	{
		bstream->read(&playerId);
		bstream->read(&cmd);
		bstream->read(&data);
		bstream->read(&unk_04);
		bstream->read(&unk_05);
		bstream->read(&unk_06);
	};

	void handle(GameConnection* conn, Player::Ptr player)
	{
		if (auto roomPlayer = player->getRoomPlayer(); roomPlayer != nullptr)
		{
			switch (cmd)
			{
			case State::INV_IN:
			case State::INV_OUT:
				roomPlayer->getRoom()->broadcastWaiting<GCGameState>(player->getId(), cmd);
				break;
			case State::LEAVE_GAME:
				if (auto roomSession = roomPlayer->getRoom()->getRoomSession(); roomSession != nullptr) // player is playing
				{
					if (!roomSession->removePlayer(player->getId()))
					{
						conn->postNetEvent(new GCGameState(player->getId(), 15));
					}
				}
				else
					conn->postNetEvent(new GCGameState(player->getId(), 15));
				
				roomPlayer->setPlaying(false);
				roomPlayer->setSpectating(false);
				roomPlayer->setReady(false);
				conn->enterRoom(roomPlayer->getRoom());
				roomPlayer->getRoom()->syncPlayers(roomPlayer);
				break;
			case State::GAME_WAITING_PLAYERS:
				if (auto roomSession = roomPlayer->getRoom()->getRoomSession(); roomSession != nullptr)
					roomSession->addPlayer(conn, roomPlayer->getTeam());
			default:
				break;
			}
		}
	}

	void process(EventConnection* ps)
	{
		post<CGGameState>(ps);
	};

	U32 playerId;
	U32 cmd;
	U32 data;
	U32 unk_04;
	U32 unk_05;
	U32 unk_06;

	TNL_DECLARE_CLASS(CGGameState);
};
#endif