#include "PveNpc.h"

#include "AABBHelper.h"
#include "gc_master_log.hpp"
#include "gc_pve_die_npc.hpp"
#include "gc_pve_npc_init.hpp"
#include "RoomSession.h"

PveNpc::PveNpc(const uint32_t type, const uint16_t baseHealth, const float speed, const uint32_t weaponItemId, const uint8_t weaponBodyPartId,
	const uint32_t aiTime, const float attackWidth, const float attackHeight, const bool shouldRespawn, const uint32_t respawnTime,
	const bool canDropLoot, const uint16_t initialRotation, const Position initialPosition,
	const eNpcGradeType gradeType, const eNpcMovementType movementType, const eNpcTargetType targetType,
	std::vector<NpcLootDrop> lootDrops, std::vector<NpcBodyPart> bodyParts) :
	m_type(type),
	m_baseHealth(baseHealth),
	m_speed(speed),
	m_health(baseHealth),
	m_weaponItemId(weaponItemId),
	m_weaponBodyPartId(weaponBodyPartId),
	m_aiTime(aiTime),
	m_attackWidth(attackWidth),
	m_attackHeight(attackHeight),
	m_shouldRespawn(shouldRespawn),
	m_respawnTime(respawnTime),
	m_canDropLoot(canDropLoot),
	m_initialRotation(initialRotation),
	m_initialPosition(initialPosition),
	m_position(initialPosition),
	m_gradeType(gradeType),
	m_movementType(movementType),
	m_targetType(targetType),
	m_lootDrops(std::move(lootDrops)),
	m_bodyParts(std::move(bodyParts))
{
}

void PveNpc::tick(const std::shared_ptr<RoomSession>& roomSession)
{
	if (isDead() && m_shouldRespawn)
	{
		const auto currentTime = time(nullptr);

		if (m_timeOfDeath == NULL)
		{
			return;
		}

		if ((m_timeOfDeath + m_respawnTime) < currentTime)
		{
			// Reset time of death.
			m_timeOfDeath = NULL;

			roomSession->getNpcManager()->respawnNpcByUid(m_uid);
		}
	}

	// Note: temporary piece of code to let npcs shoot a player when the player is in range.


	for (const auto& player : roomSession->getPlayingPlayers())
	{
		if (player->isDead())
		{
			continue;
		}

		// ReSharper disable once CppTooWideScopeInitStatement
		const auto distance = AABBHelper::getDistanceBetweenPositions(m_position, player->getPosition());

		if (distance < 7)
		{
			auto playerPosition = player->getPosition();

			constexpr auto yCorrection = 1.0f;
			playerPosition.y += yCorrection;

			attack(roomSession, playerPosition);
		}
	}
}

void PveNpc::spawn(const std::shared_ptr<RoomSession>& roomSession) const
{
	roomSession->relayPlaying<GCPvENpcInit>(m_type, m_uid, m_initialPosition, m_initialRotation);
}

void PveNpc::spawn(const std::shared_ptr<RoomSessionPlayer>& roomSessionPlayer) const
{
	roomSessionPlayer->send<GCPvENpcInit>(m_type, m_uid, m_initialPosition, m_initialRotation);
}

void PveNpc::respawn(const std::shared_ptr<RoomSession>& roomSession)
{
	resetHealth();
	spawn(roomSession);
}

void PveNpc::die(const std::shared_ptr<RoomSession>& roomSession)
{
	roomSession->relayPlaying<GCPvEDieNpc>(m_uid);

	onDeath(roomSession);
}

void PveNpc::resetHealth()
{
	m_health = m_baseHealth;
}

uint16_t PveNpc::takeDamage(const uint16_t damage)
{
	if (damage > m_health)
	{
		m_health = 0;

		return (damage - m_health);
	}

	m_health = m_health - damage;

	return damage;
}

void PveNpc::attack(const std::shared_ptr<RoomSession>& roomSession, const Position targetPosition) const
{
	roomSession->relayPlaying<GCMasterLog>(m_uid, m_weaponBodyPartId, targetPosition, 0);
}

void PveNpc::onDeath(const std::shared_ptr<RoomSession>& roomSession)
{
	if (m_shouldRespawn)
	{
		m_timeOfDeath = time(nullptr);
	}

	if (m_canDropLoot)
	{
		dropLoot(roomSession);
	}
}

void PveNpc::setUid(const uint32_t uid)
{
	m_uid = uid;
}

uint32_t PveNpc::getUid() const
{
	return m_uid;
}

uint32_t PveNpc::getType() const
{
	return m_type;
}

Position PveNpc::getInitialPosition() const
{
	return m_initialPosition;
}

Position PveNpc::getPosition() const
{
	return m_position;
}

uint16_t PveNpc::getHealth() const
{
	return m_health;
}

uint16_t PveNpc::getBaseHealth() const
{
	return m_baseHealth;
}

float PveNpc::getSpeed() const
{
	return m_speed;
}

uint16_t PveNpc::getInitialRotation() const
{
	return m_initialRotation;
}

uint32_t PveNpc::getWeaponItemId() const
{
	return m_weaponItemId;
}

bool PveNpc::shouldRespawn() const
{
	return m_shouldRespawn;
}

std::vector<NpcBodyPart> PveNpc::getBodyParts()
{
	return m_bodyParts;
}

bool PveNpc::isDead() const
{
	return m_health <= 0;
}

bool PveNpc::hasBodyPart(const uint32_t bodyPartId)
{
	for (const auto& [id, health, weaponItemId, itemBoxId, isDualGun] : m_bodyParts)
	{
		if (id == bodyPartId)
		{
			return true;
		}
	}

	return false;
}

void PveNpc::dropLoot(const std::shared_ptr<RoomSession>& roomSession)
{
	std::vector<uint32_t> itemDrops{};

	for (const auto [itemId, probability] : m_lootDrops)
	{
		for (size_t i = 0; i < probability; i++)
		{
			itemDrops.push_back(itemId);
		}
	}

	// TODO: Properly create a random seed.
	// See: https://stackoverflow.com/questions/50662280/c-need-a-good-technique-for-seeding-rand-that-does-not-use-time
	srand(time(nullptr));

	const auto randomItemType = itemDrops[rand() % itemDrops.size()];

	const auto randomPveItem = PveItem
	{
		randomItemType,
		m_position
	};

	roomSession->getPveItemManager()->spawnItem(std::make_shared<PveItem>(randomPveItem));

	m_canDropLoot = false;
}
