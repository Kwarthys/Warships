#include "CommandBuffer.h"

//do not call from main thread
void CommandBuffer::waitToAdd(std::unique_ptr<Command> commandPtr)
{
	std::unique_lock<std::mutex> theLock(mutex);//wait mutex to get free
	commands.emplace(std::move(commandPtr));
	theLock.unlock();
}

std::unique_ptr<Command> CommandBuffer::tryToGet()
{

	std::unique_lock<std::mutex> theLock(mutex, std::defer_lock);

	if (theLock.try_lock())
	{
		if (commands.size() == 0)
		{
			return nullptr;
		}
		else
		{
			std::unique_ptr<Command> commandPtr = std::move(commands.front());
			commands.pop();
			theLock.unlock();

			return commandPtr;
		}
	}
	else
	{
		return nullptr;
	}

}

void CommandBuffer::testThreading()
{
	std::unique_ptr<StringCommand> c1 = std::make_unique<StringCommand>();
	c1->id = Command::NameSend;
	c1->parameter = 1;
	c1->data = "Command1";
	c1->socketID = 265;

	std::unique_ptr<StringCommand> c2 = std::make_unique<StringCommand>();
	c2->id = Command::NameSend;
	c2->parameter = 2;
	c2->data = "Command2";
	c2->socketID = 243;

	std::unique_ptr<StringCommand> c3 = std::make_unique<StringCommand>();
	c3->id = Command::NameSend;
	c3->parameter = 3;
	c3->data = "Command3";
	c3->socketID = 272;

	CommandBuffer buffer;
	buffer.mutex.lock();
	std::thread t1(testThreadingTask, std::move(c1), std::ref(buffer));
	std::thread t2(testThreadingTask, std::move(c2), std::ref(buffer));
	std::thread t3(testThreadingTask, std::move(c3), std::ref(buffer));

	std::cout << "go" << std::endl;
	buffer.mutex.unlock();

	t1.join();
	t2.join();
	t3.join();

	std::cout << std::endl;

	while (buffer.commands.size() > 0)
	{
		std::unique_ptr<Command> c = buffer.tryToGet();
		if (c != nullptr)
		{
			CommandManager::displayCommand(c.get());
		}
	}
	
}

void CommandBuffer::testThreadingTask(std::unique_ptr<Command> toAdd, CommandBuffer& commandBuffer)
{
	int p = toAdd->parameter;
	std::cout << "adding " << p << std::endl;
	commandBuffer.waitToAdd(std::move(toAdd));
	std::cout << "Added " << p << std::endl;
}
