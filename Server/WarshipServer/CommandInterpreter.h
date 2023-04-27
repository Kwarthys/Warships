#pragma once
class CommandInterpreter
{
	public:
		CommandInterpreter(GameManager* gm) : gameManager(gm){}

	private:
		GameManager* gameManager;
};

