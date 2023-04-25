#pragma once
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <string>
#include <memory>
#include <vector>

#include "CommandManager.h"
#include "Command.h"
#include "ClientConnexion.h"

using namespace std;

class NetworkManager
{
	public:
		void startServer();
};

