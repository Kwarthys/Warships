#pragma once
class MapNode
{
	public:
		bool hit = false;
		bool taken = false;
		int shipID = -1;

		void assignShip(int shipIndex) { shipID = shipIndex; taken = true; }
};

