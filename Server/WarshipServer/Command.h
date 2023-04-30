#pragma once
#include <string>
#include <vector>

class Command
{
	public:
		enum CommandID { IDAttrib, RegisterOther, NameSend, TargetGrid, FireGrid, FireResult, ShipSunk, EndGame, PlaceShip, GameStarts };
		CommandID id;
		int parameter;
		int socketID = -1;
};

class StringCommand : public Command
{
	public:
		std::string data;
};

class IntArrayCommand : public Command
{
	public:
		std::vector<int> data;
};

