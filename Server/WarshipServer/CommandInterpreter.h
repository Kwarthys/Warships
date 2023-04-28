#pragma once

#include "GameManager.h"
#include "Command.h"
#include "CommandManager.h"

class GameManager;

class CommandInterpreter
{
	public:
		void treatCommand(const Command* command);
		void setGM(GameManager* gm) { gameManager = gm; }

	private:
		GameManager* gameManager;
};

