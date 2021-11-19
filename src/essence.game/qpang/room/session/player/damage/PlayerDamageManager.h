#pragma once

#include <memory>
#include <cstdint>

#include "CGHitInfo.h"

class RoomSessionPlayer;

class PlayerDamageManager
{
public:
	void initialize(const std::shared_ptr<RoomSessionPlayer>& roomSessionPlayer);

	/// <summary>
	/// Handles a CGHit event.
	/// </summary>
	void handleCGHit(const CGHitInfo& cgHitInfo) const;

	/// <summary>
	/// Checks if a given weaponId is a trap.
	/// </summary>
	static bool isTrap(const uint32_t weaponId);

	/// <summary>
	/// Looks up and returns the damage for the given trap.
	/// </summary>
	static uint16_t getTrapDamageById(const uint32_t trapId);

private:
	/// <summary>
	/// Handles trap damage.
	/// </summary>
	void relayTrapDamage(const CGHitInfo& cgHitInfo) const;

	//void relayCGHit()

	std::weak_ptr<RoomSessionPlayer> m_roomSessionPlayer;
};
