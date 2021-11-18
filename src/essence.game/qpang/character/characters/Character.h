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

	explicit Character(const uint16_t id, const uint16_t baseHealth, const uint8_t strength, const bool isUnlocked,
	                   const double headshotDamageModifier, const double headDamageModifier,
	                   const double bodyDamageModifier, const double armDamageModifier,
	                   const double frontalDamageModifier, const double backDamageModifier,
	                   const double backsideDamageModifier, const double specialAttackDamageModifier,
	                   const double meleeDamageModifier, const double gunDamageModifier,
	                   const double launcherDamageModifier, const double throwableDamageModifier) :
		m_id(id),

		m_baseHealth(baseHealth),
		m_strength(strength),

		m_isUnlocked(isUnlocked),

		m_headshotDamageModifier(headshotDamageModifier),

		m_headDamageModifier(headDamageModifier),
		m_bodyDamageModifier(bodyDamageModifier),
		m_armDamageModifier(armDamageModifier),

		m_frontalDamageModifier(frontalDamageModifier),
		m_backDamageModifier(backDamageModifier),
		m_backsideDamageModifier(backsideDamageModifier),

		m_specialAttackDamageModifier(specialAttackDamageModifier),

		m_meleeDamageModifier(meleeDamageModifier),
		m_gunDamageModifier(gunDamageModifier),
		m_launcherDamageModifier(launcherDamageModifier),
		m_throwableDamageModifier(throwableDamageModifier)
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
	uint16_t m_id = 0;

	uint16_t m_baseHealth = 0;
	uint8_t m_strength = 0;

	bool m_isUnlocked = false;

#pragma region BodyPart damage modifiers

	// NOTE: These damage modifiers get applied when your character gets hit.

	double m_headshotDamageModifier = 0.0;	// Specifically for snipers and launchers.

	double m_headDamageModifier = 0.0;		// For all weapon damage except snipers/launchers?
	double m_bodyDamageModifier = 0.0;
	double m_armDamageModifier = 0.0;

	double m_frontalDamageModifier = 0.0;
	double m_backDamageModifier = 0.0;
	double m_backsideDamageModifier = 0.0;

	double m_specialAttackDamageModifier = 0.0;
#pragma endregion

#pragma region Weapon damage modifiers

	// NOTE: These damage modifiers get applied when your character hits another character.

	double m_meleeDamageModifier = 0.0;
	double m_gunDamageModifier = 0.0;
	double m_launcherDamageModifier = 0.0;
	double m_throwableDamageModifier = 0.0;

#pragma endregion
};
