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
		static int serialize(const Command& c, char* buf);
		static int serializeIntArrayCommand(const IntArrayCommand& c, char* buf);
		static int serializeStringCommand(const StringCommand& c, char* buf);

		//Command* deserialize(const char const * buf, const int len) const;
		static std::unique_ptr<Command> deserialize(const char* buf, const int len);

		static void testSerialization();

		static void displayCommand(const Command& c);

	private:
		static void displayStringCommand(StringCommand& c);
		static void displayIntArrayCommand(IntArrayCommand& c);
};

