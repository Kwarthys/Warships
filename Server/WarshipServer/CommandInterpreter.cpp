#include "CommandInterpreter.h"
#include "GameManager.h"

void CommandInterpreter::treatCommand(const Command* command)
{
	 if (command->id == Command::EndGame)
	{

	}
	else if (command->id == Command::NameSend)
	{
		StringCommand* c = (StringCommand*)command;
		gameManager->setupNewPlayer(c->socketID);
		gameManager->setPlayerName(c->socketID, c->data);
	}
	else
	{
		IntArrayCommand* c = (IntArrayCommand*)command;

		switch (command->id)
		{
			case Command::PlaceShip:
				gameManager->placePlayerShip(c->socketID, (Ship::ShipType)c->parameter, (Ship::Orientation)c->data[0], c->data[1]);
				break;

			case Command::GameStarts:
				gameManager->managePlayersReady(c->socketID, c->data[0]);
				break;

			case Command::TargetGrid:
				break;

			case Command::FireGrid:
				break;

		}
	}

}
