#ifndef CG_PVE_HIT_NPC_TO_OBJECT_H
#define CG_PVE_HIT_NPC_TO_OBJECT_H

#include "GameNetEvent.h"

class CGPvEHitNpcToObject final : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U64 unk_01; // 88 92
	U32 objectUid; // 96

	CGPvEHitNpcToObject() : GameNetEvent{ CG_PVE_HIT_NPC_TO_OBJECT, GuaranteedOrdered, DirClientToServer } {}

	void pack(EventConnection* conn, BitStream* bstream) override {}

	void unpack(EventConnection* conn, BitStream* bstream) override
	{
		bstream->read(&unk_01);
		bstream->read(&objectUid);
	}

	void handle(GameConnection* conn, const Player::Ptr player) override
	{
		const auto roomPlayer = player->getRoomPlayer();

		if (roomPlayer == nullptr)
		{
			return;
		}

		const auto roomSessionPlayer = roomPlayer->getRoomSessionPlayer();

		if (roomSessionPlayer == nullptr)
		{
			return;
		}

		const auto roomSession = roomSessionPlayer->getRoomSession();

		if (roomSession == nullptr)
		{
			return;
		}

		const auto object = roomSession->getObjectManager()->findObjectByUid(objectUid);

		if (object == nullptr)
		{
			return;
		}

		object->onHitByNpc(roomSession);
	}

	void process(EventConnection* ps) override
	{
		post<CGPvEHitNpcToObject>(ps);
	}

	TNL_DECLARE_CLASS(CGPvEHitNpcToObject);
};
#endif