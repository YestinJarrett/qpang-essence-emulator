#include "CharacterManager.h"

Character CharacterManager::getCharacterById(const uint16_t id)
{
	for (const auto& character : m_characters)
	{
		if (character.getId() == id)
		{
			return character;
		}
	}

	return Character();
}

std::vector<Character> CharacterManager::getUnlockedCharacters()
{
	std::vector<Character> unlockedCharacters{};

	for (const auto& character : m_characters)
	{
		if (character.isUnlocked())
		{
			unlockedCharacters.push_back(character);
		}
	}

	return unlockedCharacters;
}

std::vector<uint16_t> CharacterManager::getUnlockedCharacterIds()
{
	std::vector<uint16_t> unlockedCharacterIds{};

	for (const auto& character : m_characters)
	{
		if (character.isUnlocked())
		{
			unlockedCharacterIds.push_back(character.getId());
		}
	}

	return unlockedCharacterIds;
}