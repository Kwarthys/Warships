#pragma once
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <atomic>
#include <thread>

#include "CommandManager.h"
#include "Command.h"
#include "ClientConnexion.h"
#include "GameManager.h"

using namespace std;

class NetworkManager
{
	public:
		void startServer();

		void sendCommandToEveryoneExcept(int exceptID, Command& c);
		void sendCommandToPlayerID(int playerID, Command& c);

		//GameManager* gameManager;

	private:
		static void welcomeClients(NetworkManager* networkManager, SOCKET serverSocket);

		std::vector<ClientConnexion> connexions;
		std::vector<int> indeciesToSockets;
		CommandBuffer inwardComs;

		std::atomic<bool> acceptMoreConnexions = true;
};

