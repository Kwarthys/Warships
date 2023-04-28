#include "CommandInterpreter.h"

void CommandInterpreter::treatCommand(const Command* command)
{
	if (command->id == Command::GameStarts)
	{

	}
	else if (command->id == Command::EndGame)
	{

	}
	else if (command->id == Command::NameSend)
	{
		StringCommand* c = (StringCommand*)command;
	}
	else
	{
		IntArrayCommand* c = (IntArrayCommand*)command;

		if (command->id == Command::PlaceShip)
		{
			gameManager->placePlayerShip(c->socketID, (Ship::ShipType)c->parameter, (Ship::Orientation)c->data[0], c->data[1]);
		}
	}

}
