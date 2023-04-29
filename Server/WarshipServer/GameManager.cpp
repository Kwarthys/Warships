#include "GameManager.h"
#include "NetworkManager.h"

void GameManager::treatCommand(const Command* command)
{
	CommandManager::displayCommand(command);
	commandInterpreter.treatCommand(command);
}

void GameManager::setupNewPlayer(int playerID)
{
	playerBoards.emplace_back(boardSizeX, boardSizeY, playerID);
	boardIndexToPlayerID.push_back(playerID);

	//warn others that this one arrived
	IntArrayCommand command;
	command.id = Command::RegisterOther;
	command.parameter = playerID;
	command.data.push_back(playerBoards.size());
	networkManager->sendCommandToEveryoneExcept(playerID, command);

	//tell this one about the others
	for (size_t i = 0; i < playerBoards.size() - 1; i++)//don't send him his own data
	{
		IntArrayCommand othersCommand;
		command.id = Command::RegisterOther;
		command.parameter = boardIndexToPlayerID.at(i);
		command.data.push_back(i);

		networkManager->sendCommandToPlayerID(playerID, othersCommand);
	}
}

void GameManager::setPlayerName(int playerID, std::string playerName)
{
	int playerIndex = getBoardIndexOfPlayer(playerID);
	playerBoards.at(playerIndex).playerName = playerName;

	//warn the others
	StringCommand nameCommand;
	nameCommand.id = Command::NameSend;
	nameCommand.parameter = playerID;
	nameCommand.data = playerName;
	networkManager->sendCommandToEveryoneExcept(playerID, nameCommand);
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
