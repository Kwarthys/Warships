#pragma once
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <string>
#include <memory>

#include "CommandManager.h"
#include "Command.h"

class ClientConnexion
{
	public:
		ClientConnexion(SOCKET associatedSocket) : clientSocket(associatedSocket) {}

		void manageClientCommunication(CommandManager& cm);

	private:
		SOCKET clientSocket;
};

