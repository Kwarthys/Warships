#include <vector>

#include "Ship.h"
#include "MapNode.h"

#pragma once
class Board
{
	public:
		Board(int sizeX, int sizeY);

		bool placeShip(Vector2 pos, Ship& ship);

		int fromCoordToIndex(int x, int y) { return y * sizeX + x; };
		int fromCoordToIndex(Vector2 coords) { return coords.y * sizeX + coords.x; };
		Vector2 fromIndexToCoord(int index);

	private:
		int sizeX;
		int sizeY;
		std::vector<MapNode> map;
		std::vector<Ship> ships;
 };

