#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <memory>

#include "Command.h"

class CommandManager
{
	public:
		int serialize(Command& c, char* buf);
		int serializeIntArrayCommand(IntArrayCommand& c, char* buf);
		int serializeStringCommand(StringCommand& c, char* buf);

		//Command* deserialize(const char const * buf, const int len) const;
		std::unique_ptr<Command> deserialize(const char const* buf, const int len) const;

		void testSerialization();

		void displayCommand(const Command& c);

	private:
		void displayStringCommand(StringCommand& c);
		void displayIntArrayCommand(IntArrayCommand& c);
};

