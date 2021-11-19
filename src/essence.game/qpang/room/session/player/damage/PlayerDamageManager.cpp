#include "PlayerDamageManager.h"

#include "cg_game_state.hpp"
#include "gc_game_state.hpp"
#include "gc_hit.hpp"

#include "Player.h"
#include "RoomSession.h"
#include "RoomSessionPlayer.h"

#include "enums/Trap.h"
#include "structs/CGHitInfo.h"

void PlayerDamageManager::initialize(const std::shared_ptr<RoomSessionPlayer>& roomSessionPlayer)
{
	m_roomSessionPlayer = roomSessionPlayer;
}

#pragma region CGHit Event

void PlayerDamageManager::handleCGHit(const CGHitInfo& cgHitInfo) const
{
	const auto roomSessionPlayer = m_roomSessionPlayer.lock();

	if (roomSessionPlayer == nullptr)
	{
		return;
	}

	if (cgHitInfo.targetPlayerId == NULL)
	{
		// TODO: Relay empty hit.

		return;
	}

	if (isTrap(cgHitInfo.weaponId))
	{
		relayTrapDamage(cgHitInfo);

		return;
	}

	// TODO: At this point, the damage is not a trap and the target player exists, so we will have to perform some
	// source and target player checks, see cg_hit.
}

#pragma endregion

#pragma region Trap Related Methods

bool PlayerDamageManager::isTrap(const uint32_t weaponId)
{
	// NOTE: The TRAP_DOWN trap is ignored since fall damage is not implemented.

	return (weaponId == TRAP_FLAME)
		|| (weaponId == TRAP_PRESS)
		|| (weaponId == TRAP_FALL)
		|| (weaponId == TRAP_POISON_GROUND)
		|| (weaponId == TRAP_SHIP_PROPELLER);
}

uint16_t PlayerDamageManager::getTrapDamageById(const uint32_t trapId)
{
	// NOTE: The TRAP_DOWN trap is ignored since fall damage is not implemented.

	switch (trapId)
	{
	case TRAP_FLAME:
	case TRAP_POISON_GROUND:
	case TRAP_SHIP_PROPELLER:
		return 10;
	case TRAP_PRESS:
		return 150;
	case TRAP_FALL:
	default:
		return 9999;
	}
}

void PlayerDamageManager::relayTrapDamage(const CGHitInfo& cgHitInfo) const
{
	const auto roomSessionPlayer = m_roomSessionPlayer.lock();
	const auto roomSession = roomSessionPlayer->getRoomSession();

	if (roomSession == nullptr)
	{
		return;
	}

	const auto targetPlayer = roomSession->find(cgHitInfo.targetPlayerId);

	if (targetPlayer == nullptr)
	{
		return;
	}

	const auto trapId = cgHitInfo.weaponId;

	if (const auto essenceHolder = roomSession->getEssenceHolder(); essenceHolder != nullptr)
	{
		const auto essenceHolderPlayerId = essenceHolder->getPlayer()->getId();
		const auto isEssenceHolder = (essenceHolderPlayerId == targetPlayer->getPlayer()->getId());

		if ((trapId == TRAP_FALL) && isEssenceHolder)
		{
			roomSession->resetEssence();
		}
	}

	const auto damage = getTrapDamageById(trapId);

	roomSessionPlayer->takeHealth(damage);

	const auto remainingHealth = roomSessionPlayer->getHealth();

	constexpr auto streak = 0;
	constexpr auto effectId = 0;

	// TODO: Create constructor specifically for trap damage.
	roomSession->relayPlaying<GCHit>(
		cgHitInfo.sourcePlayerId, cgHitInfo.targetPlayerId, cgHitInfo.unknown03,
		cgHitInfo.sourcePlayerPosition.x, cgHitInfo.sourcePlayerPosition.y, cgHitInfo.sourcePlayerPosition.z,
		cgHitInfo.targetPlayerPosition.x, cgHitInfo.targetPlayerPosition.y, cgHitInfo.targetPlayerPosition.z,
		cgHitInfo.entityId, cgHitInfo.hitType, cgHitInfo.bodyPart, remainingHealth, damage, trapId,
		cgHitInfo.rtt, cgHitInfo.weaponType, cgHitInfo.hitLocation, streak, cgHitInfo.unknown18, effectId);

	if (targetPlayer->isDead())
	{
		// TODO: Put in separate function?
		roomSession->relayPlaying<GCGameState>(cgHitInfo.targetPlayerId, CGGameState::State::KILLFEED_ADD_SUICIDE, trapId, cgHitInfo.sourcePlayerId);

		// TODO: Create onPlayerKill without weapon in parameters, since this is suicide.
		roomSession->getGameMode()->onPlayerKill(targetPlayer, targetPlayer, Weapon{}, cgHitInfo.hitLocation);
	}
}

#pragma endregion
