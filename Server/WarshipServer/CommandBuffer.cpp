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

void CommandBuffer::testThreading()
{
	std::unique_ptr<StringCommand> c1 = std::make_unique<StringCommand>();
	c1->id = Command::NameSend;
	c1->parameter = 1;
	c1->data = "Command1";

	std::unique_ptr<StringCommand> c2 = std::make_unique<StringCommand>();
	c2->id = Command::NameSend;
	c2->parameter = 2;
	c2->data = "Command2";

	std::unique_ptr<StringCommand> c3 = std::make_unique<StringCommand>();
	c3->id = Command::NameSend;
	c3->parameter = 3;
	c3->data = "Command3";

	CommandBuffer buffer;
	buffer.DEBUG_SETLOCK(true);

	std::thread t1(testThreadingTask, std::move(c1), std::ref(buffer));
	std::thread t2(testThreadingTask, std::move(c2), std::ref(buffer));
	std::thread t3(testThreadingTask, std::move(c3), std::ref(buffer));

	std::cout << "go" << std::endl;
	buffer.DEBUG_SETLOCK(false);

	t1.join();
	t2.join();
	t3.join();

	for (size_t i = 0; i < buffer.commands.size(); i++)
	{
		std::cout << buffer.commands[i]->parameter << std::endl;
	}
}

void CommandBuffer::testThreadingTask(std::unique_ptr<Command> toAdd, CommandBuffer& commandBuffer)
{
	int p = toAdd->parameter;
	std::cout << "Trying to add " << p << std::endl;
	commandBuffer.waitToAdd(std::move(toAdd));
	std::cout << "Added " << p << std::endl;
}
