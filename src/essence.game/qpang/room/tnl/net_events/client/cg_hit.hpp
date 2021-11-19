#ifndef CG_HIT_H
#define CG_HIT_H

#include "AABBHelper.h"
#include "CGHitInfo.h"
#include "GameNetEvent.h"
#include "qpang/Game.h"

class CGHit final : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	// NOTE: 10 and 11 are splash related.
	enum BodyPart : U8 {
		HEAD = 0,
		BODY = 1,
		RIGHT_ARM = 2,
		RIGHT_HAND = 3,
		LEFT_ARM = 4,
		LEFT_HAND = 5,
		RIGHT_LEG = 6,
		RIGHT_FEET = 7,
		LEFT_LEG = 8,
		LEFT_FEET = 9,
	};

	//enum HitType : U8
	//{
	//	PLAYER_HIT = 0,
	//	PLAYER_NO_HIT = 1,
	//	FALL_DAMAGE = 2
	//};

	enum HitLocation : U8 {
		FRONT = 1,
		BACK = 2,
		BACK_SIDE = 3
	};

	enum Trap : U8 {
		TRAP_FLAME = 1,
		TRAP_PRESS = 2,
		TRAP_FALL = 3,
		TRAP_DOWN = 4,
		TRAP_POISON_GROUND = 6,
		TRAP_SHIP_PROPELLER = 7
	};

	CGHit() : GameNetEvent{ CG_HIT, Guaranteed, DirClientToServer } {};

	void pack(EventConnection* conn, BitStream* bstream) override {};
	void unpack(EventConnection* conn, BitStream* bstream) override
	{
		bstream->read(&sourcePlayerId);
		bstream->read(&targetPlayerId);
		bstream->read(&unk_03);
		bstream->read(&srcPosX);
		bstream->read(&srcPosY);
		bstream->read(&srcPosZ);
		bstream->read(&dstPosX);
		bstream->read(&dstPosY);
		bstream->read(&dstPosZ);
		bstream->read(&entityId);
		bstream->read(&hitType);
		bstream->read(&bodyPart);
		bstream->read(&weaponId);
		bstream->read(&rtt);
		bstream->read(&weaponType);
		bstream->read(&hitLocation);
		bstream->read(&unk_17);
		bstream->read(&unk_18);

		std::cout << "==================================================" << std::endl;
		std::cout << "SourcePlayerId: " << (uint32_t)sourcePlayerId << ", TargetPlayerId: " << (uint32_t)targetPlayerId << std::endl;
		std::cout << "WeaponId: " << (uint32_t)weaponId << ", WeaponType: " << (uint32_t)weaponType << ", EntityId: " << (uint32_t)entityId << std::endl;
		std::cout << "HitLocation: " << (uint32_t)hitLocation << ", BodyPart: " << (uint32_t)bodyPart << ", HitType: " << (uint32_t)hitType << std::endl;
		std::cout << "RTT: " << (uint32_t)rtt << std::endl;
		std::cout << "==================================================" << std::endl;

	};

	void handle(GameConnection* conn, const Player::Ptr player) override
	{
		const auto roomPlayer = player->getRoomPlayer();

		if (roomPlayer == nullptr)
		{
			return;
		}

		if (roomPlayer->isSpectating())
		{
			return;
		}

		const auto roomSessionPlayer = roomPlayer->getRoomSessionPlayer();

		if (roomSessionPlayer == nullptr)
		{
			return;
		}

		roomSessionPlayer->getDamageManager()->handleCGHit(CGHitInfo
		{
			sourcePlayerId,
			targetPlayerId,
			Position { srcPosX, srcPosY, srcPosZ },
			Position { dstPosX, dstPosY, dstPosZ },
			entityId,
			weaponId,
			weaponType,
			hitType,
			hitLocation,
			bodyPart,
			rtt,
			unk_03,
			unk_17,
			unk_18
		});

		return;

		if (targetPlayerId == NULL)
		{
			// TODO: Destination player does not exist, send hit empty.

			return;
		}

		if (sourcePlayerId == targetPlayerId)
		{
			// TODO: Source and destination player are the same, relay the hit.

			return;
		}

		const auto targetRoomSessionPlayer = roomSessionPlayer->getRoomSession()->find(targetPlayerId);

		if (targetRoomSessionPlayer == nullptr)
		{
			return;
		}

		if (targetRoomSessionPlayer->isInvincible() || targetRoomSessionPlayer->isDead())
		{
			return;
		}

		// TODO: Relay the hit.
	}

	void process(EventConnection* ps) override
	{
		post<CGHit>(ps);
	};

	// Id's of the damage dealing player and damage receiving player.
	U32 sourcePlayerId;
	U32 targetPlayerId;

	U32 unk_03;

	// Position (x,y,z) of the damage dealing player.
	F32 srcPosX;
	F32 srcPosY;
	F32 srcPosZ;

	// Position (x,y,z) of the damage receiving player.
	F32 dstPosX;
	F32 dstPosY;
	F32 dstPosZ;

	// Id of the entity (throwables?).
	U32 entityId;

	// Need to figure these out.
	U8 hitType;
	// The bodypart of the players character that was hit.
	U8 bodyPart;

	// The id of the weapon that is used to deal damage.
	U32 weaponId;

	// Round trip time
	U64 rtt;

	// Type of the weapon that is used to deal damage.
	U8 weaponType;

	// Tells us the location on the body that was hit (front, back or backside).
	U8 hitLocation;

	// Need to figure these out.
	U8 unk_17;
	U32 unk_18;

	TNL_DECLARE_CLASS(CGHit);
};
#endif