#pragma once

#include "Command.h"
#include "CommandManager.h"
#include "CommandInterpreter.h"

class GameManager
{
	public:
		void treatCommand(Command* command);

	private:
		CommandInterpreter commandInterpreter = CommandInterpreter(this);
};

