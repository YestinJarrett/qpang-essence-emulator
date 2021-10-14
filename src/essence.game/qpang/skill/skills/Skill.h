#pragma once

#include "ItemID.h"

class Skill
{
public:
	Skill() :
		m_requiredSkillPoints(0),
		m_hasDuration(false),
		m_durationInSeconds(0)
	{
	}

	void tick()
	{
		if (m_hasDuration && m_durationInSeconds > 0)
		{
			m_durationInSeconds--;
		}
	}
	
	void bind(std::shared_ptr<RoomSessionPlayer> roomSessionPlayer)
	{
		m_roomSessionPlayer = m_roomSessionPlayer;
	}

	virtual void apply()
	{

	}

	virtual uint32_t getItemId()
	{
		return ItemID::SKILL_NONE;
	}

	virtual bool shouldDisableOnBeingAttacked()
	{
		return false;
	}

	virtual bool shouldDisableOnGameItemPickup()
	{
		return false;
	}

	virtual bool shouldDisableOnUseOfActionCard()
	{
		return false;
	}

	bool hasDuration()
	{
		return m_hasDuration;
	}

	uint32_t getDurationInSeconds()
	{
		return m_durationInSeconds;
	}

	uint32_t getRequiredSkillPoints()
	{
		return m_requiredSkillPoints;
	}
protected:
	std::shared_ptr<RoomSessionPlayer> m_roomSessionPlayer;

	bool m_hasDuration;
	uint32_t m_durationInSeconds;

	uint32_t m_requiredSkillPoints;
private:
};