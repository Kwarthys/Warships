#include "GameManager.h"
#include "Vector2.h"

void GameManager::treatCommand(const Command* command)
{
	//commandInterpreter.treatCommand(command);
}

void GameManager::setupNewPlayer(int playerID)
{
	playerBoards.emplace_back(boardSizeX, boardSizeY, playerID);
	boardIndexToPlayerID.push_back(playerID);
}

void GameManager::setPlayerName(int playerID, std::string playerName)
{
	playerBoards.at(getBoardIndexOfPlayer(playerID)).playerName = playerName;
}

bool GameManager::placePlayerShip(int playerID, Ship::ShipType shipType, Ship::Orientation facing, int rootNode)
{
	int index = getBoardIndexOfPlayer(playerID);
	return playerBoards.at(index).placeNewShip(shipType, facing, rootNode);
}

int GameManager::getBoardIndexOfPlayer(int playerID)
{
	for (size_t i = 0; i < boardIndexToPlayerID.size(); i++)
	{
		if (boardIndexToPlayerID.at(i) == playerID)
		{
			return i;
		}
	}

	std::cout << "Couldn't find board of Player" << playerID << std::endl;
	return -1;
}
