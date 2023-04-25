#include "CommandManager.h"

int CommandManager::serialize(Command& c, char* buf)
{
	if (c.id == Command::NameSend)
	{
		return serializeStringCommand((StringCommand&)c, buf);
	}
	else
	{
		return serializeIntArrayCommand((IntArrayCommand&)c, buf);
	}
}


int CommandManager::serializeIntArrayCommand(IntArrayCommand& c, char* buf)
{
	int neededLen = 2 + c.data.size();
	
	buf[0] = (unsigned char)c.id;
	buf[1] = (unsigned char)c.parameter;

	for (size_t i = 0; i < c.data.size(); i++)
	{
		buf[i + 2] = c.data.at(i);
	}

	return neededLen;
}


int CommandManager::serializeStringCommand(StringCommand& c, char* buf)
{
	int neededLen = 2 + c.data.size();

	buf[0] = (unsigned char)c.id;
	buf[1] = (unsigned char)c.parameter;

	for (size_t i = 0; i < c.data.size(); i++)
	{
		buf[i + 2] = c.data.at(i);
	}

	return neededLen;
}


std::unique_ptr<Command> CommandManager::deserialize(const char * buf, const int len) const
{
	Command::CommandID id = (Command::CommandID)buf[0];

	std::unique_ptr<Command> commandPtr;

	if (id == Command::NameSend)
	{
		//Deserialize string
		std::unique_ptr<StringCommand> stringCommandPtr = std::make_unique<StringCommand>();

		stringCommandPtr->id = id;
		stringCommandPtr->parameter = buf[1];

		std::ostringstream stream;
		
		for (int i = 2; i < len; i++)
		{
			stream << buf[i];
		}

		stringCommandPtr->data = stream.str();

		commandPtr = std::move(stringCommandPtr);
	}
	else
	{
		//Deserialize int array
		std::unique_ptr<IntArrayCommand> intArrayCommandPtr = std::make_unique <IntArrayCommand>();

		intArrayCommandPtr->id = id;
		intArrayCommandPtr->parameter = buf[1];

		for (int i = 2; i < len; i++)
		{
			intArrayCommandPtr->data.push_back(buf[i]);
		}

		commandPtr = std::move(intArrayCommandPtr);
	}


	return commandPtr;
}


void CommandManager::testSerialization()
{
	char buf[32];
	StringCommand c;
	c.id = Command::NameSend;
	c.parameter = 0;
	c.data = "Zykyflex";

	int len = serialize(c, buf);

	for(int i = 0; i < len; ++i)
	{
		std::cout << (int)buf[i] << " ";
	}

	std::cout << std::endl;

	std::unique_ptr<Command> cptr = deserialize(buf, len);
	displayCommand(*cptr);

	IntArrayCommand ic;
	ic.id = Command::TargetGrid;
	ic.parameter = 2;
	
	for (size_t i = 0; i < 5; i++)
	{
		ic.data.push_back(i*2 + 2);
	}

	len = serialize(ic, buf);

	for (int i = 0; i < len; ++i)
	{
		std::cout << (int)buf[i] << " ";
	}
	std::cout << std::endl;

	cptr = deserialize(buf, len);
	displayCommand(*cptr);
}

void CommandManager::displayCommand(const Command& c)
{
	if (c.id == Command::NameSend)
	{
		displayStringCommand((StringCommand&)c);
	}
	else
	{
		displayIntArrayCommand((IntArrayCommand&)c);
	}
}

void CommandManager::displayStringCommand(StringCommand& c)
{
	std::ostringstream stream;
	stream << c.id << " " << c.parameter << " s" << c.socketID << " : " << c.data << std::endl;

	std::cout << stream.str();
}

void CommandManager::displayIntArrayCommand(IntArrayCommand& c)
{
	std::ostringstream stream;
	stream << c.id << " " << c.parameter << " s" << c.socketID << " : ";

	for (size_t i = 0; i < c.data.size(); i++)
	{
		stream << c.data.at(i) << " ";
	}

	std::cout << stream.str() << std::endl;
}
