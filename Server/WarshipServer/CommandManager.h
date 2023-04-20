#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

#include "Command.h"

class CommandManager
{
	public:
		int serialize(Command& c, char* buf);
		int serializeIntArrayCommand(IntArrayCommand& c, char* buf);
		int serializeStringCommand(StringCommand& c, char* buf);

		Command* deserialize(char* buf, int len);

		void testSerialization();

		void displayCommand(Command& c);

	private:
		void displayStringCommand(StringCommand& c);
		void displayIntArrayCommand(IntArrayCommand& c);
};

