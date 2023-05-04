#pragma once

#include <vector>
#include <iostream>

#include "Command.h"
#include "CommandManager.h"
#include "Board.h"
#include "Ship.h"
#include "Vector2.h"
#include "CommandInterpreter.h"

class NetworkManager;

class GameManager
{
	public:
		GameManager() : networkManager(nullptr)	{ commandInterpreter.setGM(this); }

		void treatCommand(const Command* command);

		void setupNewPlayer(const int playerID);
		void setPlayerName(const int playerID, const std::string playerName);
		bool placePlayerShip(const int playerID, const Ship::ShipType shipType, const Ship::Orientation facing, const int rootNode);

		void managePlayersReady(int playerID, bool readyStatus);

		void managePlayerTargeting(int attackingPlayerID, int attackedPlayerID, int attackedNodeIndex);
		void managePlayerFire(int attackingPlayerID, std::vector<int> targetPairs);

		NetworkManager* networkManager;

	private:
		int getBoardIndexOfPlayer(int playerID);

		void computeTargetingDamage();
		void sendDamageResults();
		void clearTargetingResultHelpers();

		CommandInterpreter commandInterpreter;

		std::vector<int> boardIndexToPlayerID;
		std::vector<Board> playerBoards;

		int boardSizeX = 10;
		int boardSizeY = 10;
};

