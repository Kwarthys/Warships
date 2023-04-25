#pragma once
#include <string>
#include <vector>

class Command
{
	public:
		enum CommandID { IDAttrib, RegisterOther, NameSend, TargetGrid, FireGrid, FireResult, ShipSunk, EndGame };
		CommandID id;
		int parameter;
		int socketID;
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

