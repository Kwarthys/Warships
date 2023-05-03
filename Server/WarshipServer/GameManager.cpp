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

void GameManager::managePlayersReady(int playerID, bool readyStatus)
{

	int index = getBoardIndexOfPlayer(playerID);
	playerBoards.at(index).playerReady = readyStatus;

	//Check if everyone is ready
	bool everyOneReady = readyStatus;
	for (size_t i = 0; i < playerBoards.size() && everyOneReady; i++)
	{
		everyOneReady = everyOneReady && playerBoards.at(i).playerReady;
	}

	if (everyOneReady)
	{
		//warn everyone
		std::cout << "Warning everyone that game starts" << endl;
		IntArrayCommand command;
		command.id = Command::GameStarts;
		command.parameter = 0; //everyone ready
		command.data.push_back(0);//padding for easier client management
		command.socketID = -3;//won't be sent, just for monitoring

		networkManager->sendCommandToEveryone(command);
	}
	else
	{
		//warn the others
		std::cout << "Warning others about " << playerID << "'s " << readyStatus << " readyState" << endl;
		IntArrayCommand command;
		command.id = Command::GameStarts;
		command.parameter = playerID;
		command.socketID = -2;//won't be sent, just for monitoring
		command.data.push_back(readyStatus ? 1 : 0);
		networkManager->sendCommandToEveryoneExcept(playerID, command);
	}

	
}

void GameManager::managePlayerTargeting(int attackingPlayerID, int attackedPlayerID, int attackedNodeIndex)
{
	IntArrayCommand command;
	command.id = Command::TargetGrid;
	command.parameter = attackingPlayerID;
	command.socketID = -2; //Exclusive broadcast code
	command.data.push_back(attackedPlayerID);
	command.data.push_back(attackedNodeIndex);

	//warn the others
	std::cout << "Warning others about " << attackingPlayerID << " attacking " << attackedPlayerID << " at " << attackedNodeIndex  << endl;
	networkManager->sendCommandToEveryoneExcept(attackingPlayerID, command);
}

void GameManager::managePlayerFire(int attackingPlayerID, std::vector<int> targetPairs)
{
	int attackingPlayerIndex = getBoardIndexOfPlayer(attackingPlayerID);

	if (playerBoards.at(attackingPlayerIndex).playerReady)
	{
		return;
	}

	IntArrayCommand command;
	command.id = Command::FireReady;
	command.parameter = attackingPlayerID;
	command.socketID = -2;

	bool playerReady = targetPairs.at(0) != 0;

	if (playerReady)
	{
		playerBoards.at(attackingPlayerIndex).targetPairs = std::vector<int>(targetPairs);
	}

	playerBoards.at(attackingPlayerIndex).playerReady = playerReady;
	command.data.push_back(playerReady);

	//warn the others
	std::cout << "Warning others that Player" << attackingPlayerID << " is " << (playerReady ? "" : "not ") << "ready" << endl;
	networkManager->sendCommandToEveryoneExcept(attackingPlayerID, command);

	bool allReady = playerReady;
	for (size_t i = 0; i < playerBoards.size() && allReady; i++)
	{
		allReady = allReady && playerBoards.at(i).playerReady;
	}

	if (allReady)
	{
		//Compute damage and send results
	}
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
