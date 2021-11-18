#pragma once

#include <unordered_map>
#include <cstdint>
#include <mutex>

#include "core/database/Database.h"

#include "qpang/weapon/Weapon.h"

class WeaponManager
{
public:
	void initialize();

	bool canEquip(uint32_t weaponId, uint16_t characterId);
	Weapon get(uint32_t weaponId);

	static WeaponType getWeaponType(uint32_t weaponType);
private:
	std::mutex m_weaponMx;
	std::unordered_map<uint32_t, Weapon> m_weapons;
};