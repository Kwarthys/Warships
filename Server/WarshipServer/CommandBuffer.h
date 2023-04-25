#pragma once

#include <vector>
#include <memory>

#include "Command.h"

class CommandBuffer
{
	public:
		void waitToAdd(std::unique_ptr<Command> commandPtr);
		bool tryToGet(std::unique_ptr<Command> commandPtr);

	private:
		std::vector<std::unique_ptr<Command>> commands;
		volatile bool locked = false;
};

