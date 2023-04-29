#pragma once

#include "CommandManager.h"
#include "Command.h"

class GameManager;

class CommandInterpreter
{
	public:
		void treatCommand(const Command* command);
		void setGM(GameManager* gm) { gameManager = gm; }

	private:
		GameManager* gameManager;
};

