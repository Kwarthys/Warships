#pragma once

#include <vector>
#include <iostream>

#include "Command.h"
#include "CommandManager.h"
#include "CommandInterpreter.h"
#include "Board.h"
#include "Ship.h"
#include "NetworkManager.h"

class GameManager
{
	public:
		//GameManager() { commandInterpreter.setGM(this); }

		void treatCommand(const Command* command);

		void setupNewPlayer(int playerID);
		void setPlayerName(int playerID, std::string playerName);
		bool placePlayerShip(int playerID, Ship::ShipType shipType, Ship::Orientation facing, int rootNode);

		//NetworkManager* networkManager;

	private:
		int getBoardIndexOfPlayer(int playerID);

		//CommandInterpreter commandInterpreter;

		std::vector<int> boardIndexToPlayerID;
		std::vector<Board> playerBoards;

		int boardSizeX = 10;
		int boardSizeY = 10;
};

