#pragma once
#include <string>
#include <vector>

class Command
{
	public:
		enum CommandID { IDAttrib, NameSend, TargetGrid, FireGrid, FireResult, ShipSunk, EndGame };
		CommandID id;
		int parameter;
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

