#pragma once
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <string>

#include "CommandManager.h"
#include "Command.h"

using namespace std;

class NetworkManager
{
	public:
		void startServer(CommandManager& cm);
};

