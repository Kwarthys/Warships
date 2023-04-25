#pragma once
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <string>
#include <memory>
#include <thread>

#include "CommandManager.h"
#include "Command.h"
#include "CommandBuffer.h"

class ClientConnexion
{
	public:
		ClientConnexion(SOCKET associatedSocket, CommandBuffer& inwardCommandBuffer) :
			clientSocket(associatedSocket), inwardCommandBuffer(inwardCommandBuffer) {}

		void startClientListening();

		void sendToClient(const Command& c) const;
		std::thread listenerThread;

	private:
		SOCKET clientSocket;
		CommandBuffer& inwardCommandBuffer;
		static void manageClientCommunication(const ClientConnexion& clientConnexion);

};

