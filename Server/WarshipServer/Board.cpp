#include "Board.h"

Board::Board(int sizeX, int sizeY, int playerID)
{
	this->playerID = playerID;
	this->sizeX = sizeX;
	this->sizeY = sizeY;
	int totalSize = sizeX * sizeY;

	map.resize((size_t)totalSize);

	for (size_t i = 0; i < map.size(); ++i)
	{
		map.at(i) = MapNode();
	}
}

int Board::shoot(int nodeIndex)
{
	Vector2 target = fromIndexToCoord(nodeIndex);

	bool shipHit = map.at(nodeIndex).taken && !map.at(nodeIndex).hit;

	map.at(nodeIndex).hit = true;

	int shipIndex = 0;

	if (shipHit)
	{
		shipIndex = map.at(nodeIndex).shipID;

		debugDisplayMap();
		std::cout << "hit shipIndex:" << shipIndex << std::endl;

		ships.at(shipIndex-1).takeHit(target);//shipIndex 0 does not exist

		if (ships.at(shipIndex - 1).isSunk())
		{
			newlySunkedShip.push_back(shipIndex);
		}
	}

	return shipIndex;
}

int Board::shoot(Vector2 target)
{
	int nodeIndex = fromCoordToIndex(target);

	return shoot(nodeIndex);
}

bool Board::placeNewShip(Ship::ShipType shipType, Ship::Orientation facing, int rootNode)
{
	Ship toplace(fromIndexToCoord(rootNode), Ship::typeToLength(shipType), facing);
	return placeShip(toplace);
}

bool Board::placeShip(Ship ship)
{
	int targetedIndex = fromCoordToIndex(ship.pos);

	if (isSpotFree(ship.footprint))
	{
		ship.index = ships.size()+1;//avoid index 0
		ships.emplace_back(ship);
		assignShipToMap(ship);

		debugDisplayMap();
	}

	return false;
}

void Board::assignShipToMap(Ship& ship)
{
	//spot is already free
	Footprint f = ship.footprint;
	for (int j = 0; j < f.h; ++j)
	{
		for (int i = 0; i < f.w; ++i)
		{
			int x = f.x + i;
			int y = f.y + j;

			int nodeIndex = fromCoordToIndex(x, y);
			map.at(nodeIndex).assignShip(ship.index);
		}
	}
}

bool Board::isSpotFree(Footprint& spot)
{
	std::cout << "x:" << spot.x << " y:" << spot.y << " w:" << spot.w << " h:" << spot.h << std::endl;

	bool available = false;
	/*** Check map boundaries ***/
	if (spot.x >= 0 && spot.x + spot.w - 1 < sizeX && spot.y >= 0 && spot.y + spot.h - 1 < sizeY)
	{
		available = true;

		for (int j = 0; j < spot.h && available; ++j)
		{
			for (int i = 0; i < spot.w; ++i)
			{
				//a bit overkill as ships are only one tile wide, but that will allow to test areas later, or add bigger ships ?
				int x = spot.x + i;
				int y = spot.y + j;

				int nodeIndex = fromCoordToIndex(x, y);
				available = available && !map.at(nodeIndex).taken; //if one is false, free will be false and loop terminated
			}
		}
	}
	else
	{
		std::cout << "Ship outside boundaries" << std::endl;
	}

	return available;
}

Vector2 Board::fromIndexToCoord(int index)
{
	//will have to check that
	Vector2 coords = Vector2();
	coords.x = index % sizeX;
	coords.y = index / sizeX;

	return coords;
}

int Board::getNumberOfShipsAfloat()
{
	int afloat = 0;
	for (size_t i = 0; i < ships.size(); i++)
	{
		if (!ships.at(i).isSunk())
		{
			afloat++;
		}
	}

	return afloat;
}

bool Board::isShipSunk(int shipIndex)
{
	return ships.at(shipIndex - 1).isSunk();
}

void Board::debugDisplayMap()
{
	std::ostringstream text;

	text << "Player " << playerID << ":" << std::endl;

	for (int j = sizeY - 1; j >= 0; --j)
	{
		for (int i = 0; i < sizeX; ++i)
		{
			int nodeIndex = fromCoordToIndex(i, j);
			int id = map.at(nodeIndex).shipID;
			std::string idString;

			if (id == -1)
			{
				idString = "~~~";
			}
			else
			{
				idString = std::to_string(id);
				if (map.at(nodeIndex).hit)
				{
					idString = "x" + idString;
				}
				while (idString.size() < 3) { idString = " " + idString; }
			}
			text << idString << " ";
		}
		text << std::endl;
	}
	std::cout << text.str() << std::endl;
}
