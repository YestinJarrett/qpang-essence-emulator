#pragma once

#include <cstdint>
#include <memory>
#include <array>
#include <unordered_map>

#include "qpang/weapon/Weapon.h"

class RoomSessionPlayer;

class PlayerWeaponManager
{
public:
	PlayerWeaponManager();

	void initialize(std::shared_ptr<RoomSessionPlayer> player);

	Weapon getSelectedWeapon();

	void reset();
	void reload(uint32_t seqId = 0);
	void shoot(uint32_t entityId);

	bool canReload();
	bool canShoot();

	bool hasWeapon(uint32_t weaponId);
	void switchWeapon(uint32_t weaponId);
	bool isHoldingMelee();

	void refillCurrentWeapon();

	std::array<uint32_t, 4> getWeaponIds();
	std::array<Weapon, 4> getWeapons();

private:
	std::array<Weapon, 4> m_weapons;
	uint8_t m_selectedWeaponIndex;

	std::unordered_map<uint32_t, std::pair<uint16_t, uint16_t>> m_defaultAmmo;

	std::weak_ptr<RoomSessionPlayer> m_player;
};