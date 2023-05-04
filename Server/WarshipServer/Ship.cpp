#include "Ship.h"

Ship::Ship(Vector2 pos, int length, Orientation orientation) : pos(pos), length(length), facing(orientation)
{
	for (int i = 0; i < length; ++i)
	{
		hitPoints.push_back(false);
	}

	std::cout << "Placing ship at " << pos.x << "," << pos.y << std::endl;

	/*** Compute Footprint ***/
	int w = 1;
	int h = 1;
	int x = pos.x;
	int y = pos.y;
	switch (facing)
	{
	case UP:
		h = length;
		break;

	case DOWN:
		h = length;
		y = y - (length - 1);
		break;

	case RIGHT:
		w = length;
		break;

	case LEFT:
		w = length;
		x = x - (length - 1);
		break;
	}

	footprint = Footprint(x, y, w, h);
}

bool Ship::takeHit(Vector2 hit)
{
	std::cout << "hit at " << hit.x << "," << hit.y << std::endl;

	int hitPlace = fromPointToLength(hit);

	std::cout << "place is " << hitPlace << std::endl;

	if (hitPlace == -1)
	{
		std::cerr << "Ship was not hit" << std::endl;
		return false;
	}

	if (hitPoints.at(hitPlace))
	{
		std::cerr << "Ship was already hit here" << std::endl;
		return false;
	}

	hitPoints.at(hitPlace) = true;

	for (size_t i = 0; i < hitPoints.size(); i++)
	{
		std::cout << hitPoints.at(i) << " ";
	}

	std::cout << std::endl;

	if (isSunk())
	{
		std::cout << "SUNK" << std::endl;
	}

	return true;
}

int Ship::fromPointToLength(Vector2 hit)
{
	int dx = hit.x - pos.x;
	int dy = hit.y - pos.y;
	int value = -1;
	if (facing == UP || facing == DOWN)
	{
		if (dx == 0)
		{
			if (abs(dy) < length)
			{
				value = facing == UP ? dy : -dy;
			}
		}		
	}
	else if (facing == RIGHT || facing == LEFT)
	{
		if (dy == 0)
		{
			if (abs(dx) < length)
			{
				value = facing == RIGHT ? dx : -dx;
			}
		}
	}

	std::cout << "ShipIndex:" << index << " posx:" << pos.x << " posy:" << pos.y << "dx:" << dx << " dy:" << dy << "f:" << facing << " v:" << value << std::endl;

	return value;
}

bool Ship::isSunk()
{
	bool sunk = true;
	for (size_t i = 0; i < hitPoints.size(); ++i)
	{
		sunk = sunk && hitPoints.at(i);//if any section is alive (false), sunk will be false as well
	}
	return sunk;
}

int Ship::typeToLength(ShipType type)
{
	switch (type)
	{
		case ShipType::Destroyer:
			return 2;
		case ShipType::Submarine:
		case ShipType::Cruiser:
			return 3;
		case ShipType::Battleship:
			return 4;
		case ShipType::Carrier:
			return 5;
		case ShipType::None:
		default:
			return -1;
	}
}
