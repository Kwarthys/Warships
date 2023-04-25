#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <thread>
#include <atomic>

#include "Command.h"

class CommandBuffer
{
	public:
		void waitToAdd(std::unique_ptr<Command> commandPtr);
		bool tryToGet(std::unique_ptr<Command> commandPtr);

		static void testThreading();
		static void testThreadingTask(std::unique_ptr<Command> toAdd, CommandBuffer& commandBuffer);

	private:
		void DEBUG_SETLOCK(bool lockStatus) { locked = lockStatus; }
		std::vector<std::unique_ptr<Command>> commands;
		volatile std::atomic<bool> locked = false;
};

