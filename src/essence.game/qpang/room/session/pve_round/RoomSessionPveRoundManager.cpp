#include "RoomSessionPveRoundManager.h"

#include "gc_pve_new_round.hpp"
#include "gc_pve_round_end.hpp"
#include "RoomSession.h"

#include <qpang/helpers/AABBHelper.h>

#include "cg_game_state.hpp"
#include "gc_game_state.hpp"
#include "Maps.h"

void RoomSessionPveRoundManager::initialize(const std::shared_ptr<RoomSession>& roomSession)
{
	m_roomSession = roomSession;
	m_currentMap = roomSession->getRoom()->getMap();
}

void RoomSessionPveRoundManager::onStart() const
{
	updatePathfinders();
}

void RoomSessionPveRoundManager::updatePathfinders() const
{
	const auto roomSession = m_roomSession.lock();
	if (roomSession == nullptr)
		return;

	switch (m_currentRound)
	{
	case eRound::CHESS_CASTLE_STAGE_1:
		roomSession->getAboveGroundPathfinder()->updateMapInfo(Maps::pveStage1AboveGroundMapInfo);
		roomSession->getUnderGroundPathfinder()->updateMapInfo(Maps::pveStage1UnderGroundMapInfo);
		break;
	case eRound::CHESS_CASTLE_STAGE_2:
		roomSession->getAboveGroundPathfinder()->updateMapInfo(Maps::pveStage2MapInfo);
		roomSession->getUnderGroundPathfinder()->free();
		break;
	case eRound::CHESS_CASTLE_STAGE_3:
		//roomSession->getAboveGroundPathfinder()->updateMapInfo(Maps::pveStage3MapInfo);
		roomSession->getAboveGroundPathfinder()->free();
		roomSession->getUnderGroundPathfinder()->free();
		break;
	}
}

void RoomSessionPveRoundManager::onStartNewRound(const std::shared_ptr<RoomSessionPlayer>& roomSessionPlayer)
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return;
	}

	m_initializedPlayerCount++;

	// Check if the initialized player count equals the playing players size.
	if (m_initializedPlayerCount >= roomSession->getPlayingPlayers().size())
	{
		// Relay the new round to all players.
		roomSession->relayPlaying<GCPvENewRound>();

		// Reset the time.
		roomSession->resetTime();

		// Initialize all areas,objects, npcs and items.
		roomSession->getPveAreaManager()->initializeAreas();
		roomSession->getObjectManager()->initializeObjects();
		roomSession->getNpcManager()->initializeNpcs();
		roomSession->getPveItemManager()->initializeItems();

		if (m_currentRound == eRound::CHESS_CASTLE_STAGE_2)
		{
			roomSession->getPveWaveManager()->initializeWaves();
		}

		for (const auto& player : roomSession->getPlayingPlayers())
		{
			player->respawn();
			// Synchronize time for every player.
			player->send<GCGameState>(player->getPlayer()->getId(), CGGameState::State::SYNC_TIME, roomSession->getElapsedTime());
		}
	}
}

void RoomSessionPveRoundManager::endRound()
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return;
	}

	m_hasRoundEnded = true;

	roomSession->getPveAreaManager()->removeAll();
	roomSession->getObjectManager()->removeAll();
	roomSession->getNpcManager()->removeAll();
	roomSession->getPveItemManager()->removeAll();
	roomSession->getPveWaveManager()->removeAll();

	// Initiate stuff for new round.
	switch (m_currentRound)
	{
	case eRound::CHESS_CASTLE_STAGE_1:
		m_currentRound = eRound::CHESS_CASTLE_STAGE_2;
		break;
	case eRound::CHESS_CASTLE_STAGE_2:
		m_currentRound = eRound::CHESS_CASTLE_STAGE_3;
		break;
	case eRound::CHESS_CASTLE_STAGE_3:
		// TODO: Finish the game, since this is the last stage.
		return;
	}

	m_currentMap++;

	m_initializedPlayerCount = 0;
	m_hasRoundEnded = false;

	updatePathfinders();

	roomSession->stopTime();

	for (const auto& player : roomSession->getPlayingPlayers())
	{
		// Synchronize time for every player.
		player->send<GCGameState>(player->getPlayer()->getId(), CGGameState::State::SYNC_TIME, roomSession->getElapsedTime());
	}

	// Relay the round ending to all players.
	roomSession->relayPlaying<GCPvERoundEnd>();
}

void RoomSessionPveRoundManager::tick()
{
	if (m_hasRoundEnded)
	{
		return;
	}

	switch (m_currentRound)
	{
	case eRound::CHESS_CASTLE_STAGE_1:
		checkRoundZeroFinished();
		break;
	case eRound::CHESS_CASTLE_STAGE_2:
		checkRoundOneFinished();
		break;
	case eRound::CHESS_CASTLE_STAGE_3:
		break;
	}
}

void RoomSessionPveRoundManager::checkRoundZeroFinished()
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return;
	}

	const auto players = roomSession->getPlayingPlayers();

	const int finishesNeeded = static_cast<int>(players.size());

	int deadFinishes = 0;
	int actualFinishes = 0;

	constexpr auto boundsMin = Position{ 24.37f, 0.f, 25.8f };
	constexpr auto boundsMax = Position{ 40.92f, 50.f, 38.54f };

	for (const auto& player : players)
	{
		if (AABBHelper::isPositionInBetweenBounds(player->getPosition(), boundsMin, boundsMax))
		{
			actualFinishes++;
		}
		else if (player->isPermanentlyDead())
		{
			deadFinishes++;
		}
	}

	if (actualFinishes == 0)
	{
		return;
	}

	// ReSharper disable once CppTooWideScope
	const bool finishedStage = actualFinishes + deadFinishes == finishesNeeded;

	if (finishedStage)
	{
		//endRound();
		roomSession->finishPveGame(true);
		m_currentRound = eRound::CHESS_CASTLE_STAGE_2; // just for now, when we later call endRound again this is not needed

		return;
	}

	const auto currentTime = time(nullptr);

	if (currentTime >= roomSession->getEndTime())
	{
		roomSession->finishPveGame(false);
	}
}

void RoomSessionPveRoundManager::checkRoundOneFinished() const
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return;
	}

	uint32_t deadPlayerCount = 0;

	const auto players = roomSession->getPlayingPlayers();

	for (const auto& player : players)
	{
		if (player->isPermanentlyDead())
		{
			deadPlayerCount++;
		}
	}

	if (players.size() == deadPlayerCount)
	{
		// All players are dead without being able to respawn so we can finish the game.
		roomSession->finishPveGame(false);

		return;
	}

	// Note: Essence object should always be uid 1.
	const auto essenceObject = roomSession->getObjectManager()->findObjectByUid(1);

	if (essenceObject == nullptr)
	{
		return;
	}

	if (essenceObject->getHealth() == 0)
	{
		// The essence object has no health left, this means the players have lost.
		roomSession->finishPveGame(false);

		return;
	}

	const auto currentTime = time(nullptr);

	if (currentTime >= roomSession->getEndTime())
	{
		roomSession->finishPveGame(false);
	}
}

uint8_t RoomSessionPveRoundManager::getMap() const
{
	return m_currentMap;
}

eRound RoomSessionPveRoundManager::getCurrentRound() const
{
	return m_currentRound;
}
