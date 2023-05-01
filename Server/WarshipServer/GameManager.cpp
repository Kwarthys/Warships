#include "GameManager.h"
#include "NetworkManager.h"

void GameManager::treatCommand(const Command* command)
{
	CommandManager::displayCommand(command);
	commandInterpreter.treatCommand(command);
}

void GameManager::setupNewPlayer(const int playerID)
{
	playerBoards.emplace_back(boardSizeX, boardSizeY, playerID);
	boardIndexToPlayerID.push_back(playerID);

	std::cout << "Warning others that " << playerID << " arrived" << endl;
	//warn others that this one arrived
	IntArrayCommand command;
	command.id = Command::RegisterOther;
	command.parameter = playerID;
	command.socketID = -2;
	command.data.push_back(playerBoards.size()-1);
	networkManager->sendCommandToEveryoneExcept(playerID, command);

	//tell this one about the others
	for (size_t i = 0; i < playerBoards.size() - 1; i++)//don't send him his own data
	{
		std::cout << "Warning " << playerID << " about others IDs" << endl;
		IntArrayCommand othersCommand;
		othersCommand.id = Command::RegisterOther;
		othersCommand.socketID = playerID;
		othersCommand.parameter = boardIndexToPlayerID.at(i);
		othersCommand.data.push_back(i);

		networkManager->sendCommandToPlayerID(playerID, othersCommand);


		std::cout << "Warning " << playerID << " about others names" << endl;
		StringCommand othersNameCommand;
		othersNameCommand.id = Command::NameSend;
		othersNameCommand.socketID = playerID;
		othersNameCommand.parameter = boardIndexToPlayerID.at(i);
		othersNameCommand.data = playerBoards.at(i).playerName;

		networkManager->sendCommandToPlayerID(playerID, othersNameCommand);
	}
}

void GameManager::setPlayerName(const int playerID, const std::string playerName)
{
	int playerIndex = getBoardIndexOfPlayer(playerID);
	playerBoards.at(playerIndex).playerName = playerName;

	//warn the others
	std::cout << "Warning others about " << playerID << "'s name" << endl;
	StringCommand nameCommand;
	nameCommand.id = Command::NameSend;
	nameCommand.parameter = playerID;
	nameCommand.socketID = -2;
	nameCommand.data = playerName;
	networkManager->sendCommandToEveryoneExcept(playerID, nameCommand);
}

bool GameManager::placePlayerShip(const int playerID, const Ship::ShipType shipType, const Ship::Orientation facing, const int rootNode)
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
