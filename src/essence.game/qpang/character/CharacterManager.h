// ReSharper disable CppClangTidyClangDiagnosticExitTimeDestructors
#pragma once

#include <vector>

#include "Character.h"
#include "DrUruCharacter.h"
#include "HanaCharacter.h"
#include "KenCharacter.h"
#include "KumaCharacter.h"
#include "MiuMiuCharacter.h"
#include "SaiCharacter.h"

class CharacterManager
{
public:
	static Character getCharacterById(uint16_t id);

	static std::vector<Character> getUnlockedCharacters();
	static std::vector<uint16_t> getUnlockedCharacterIds();
private:
	inline const static std::vector<Character> m_characters{
		static_cast<Character>(KenCharacter()),
		static_cast<Character>(HanaCharacter()),
		static_cast<Character>(KumaCharacter()),
		static_cast<Character>(MiuMiuCharacter()),
		static_cast<Character>(SaiCharacter()),
		static_cast<Character>(DrUruCharacter())
	};
};
