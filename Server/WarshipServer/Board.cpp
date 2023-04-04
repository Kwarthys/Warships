#include "Board.h"

Board::Board(int sizeX, int sizeY)
{
	this->sizeX = sizeX;
	this->sizeY = sizeY;
	int totalSize = sizeX * sizeY;

	map.resize((size_t)totalSize);

	for (size_t i = 0; i < map.size(); ++i)
	{
		map.at(i) = MapNode();
	}
}

bool Board::placeShip(Vector2 pos, Ship& ship)
{
	int targetedIndex = fromCoordToIndex(pos);

	return false;
}

Vector2 Board::fromIndexToCoord(int index)
{
	//will have to check that
	Vector2 coords = Vector2();
	coords.x = index % sizeX;
	coords.y = index / sizeX;

	return coords;
}
