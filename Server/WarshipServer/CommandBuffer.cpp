#include "CommandBuffer.h"

//do not call from main thread
void CommandBuffer::waitToAdd(std::unique_ptr<Command> commandPtr)
{
	while (locked);

	locked = true;
	commands.push_back(std::move(commandPtr));
	locked = false;
}


bool CommandBuffer::tryToGet(std::unique_ptr<Command> commandPtr)
{
	if (locked)
	{
		return false;
	}
	else
	{
		locked = true;
		commandPtr = std::move(commands.front());
		commands.erase(commands.cbegin());
		locked = false;
		return true;
	}

}
