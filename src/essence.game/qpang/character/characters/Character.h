// ReSharper disable CppClangTidyCppcoreguidelinesSpecialMemberFunctions
#pragma once

#include <cstdint>

struct Character
{
	enum CharacterId
	{
		KEN = 333,
		HANA = 343,
		KUMA = 578,
		MIUMIU = 579,
		SAI = 850,
		DR_URU = 851,
	};

	explicit Character(const uint16_t id, const uint16_t baseHealth = 0, const uint8_t strength = 0,
						const bool isUnlocked = false) :
		m_id(id),
		m_baseHealth(baseHealth),
		m_strength(strength),
		m_isUnlocked(isUnlocked)
	{
	}

	Character() = default;

#pragma region Getters

	[[nodiscard]]
	uint16_t getId() const
	{
		return m_id;
	}

	[[nodiscard]]
	uint16_t getBaseHealth() const
	{
		return m_baseHealth;
	}

	[[nodiscard]]
	uint8_t getStrength() const
	{
		return m_strength;
	}

	[[nodiscard]]
	uint8_t isUnlocked() const
	{
		return m_isUnlocked;
	}

#pragma endregion

protected:
	uint16_t m_id;

	uint16_t m_baseHealth;
	uint8_t m_strength;

	bool m_isUnlocked;
};
