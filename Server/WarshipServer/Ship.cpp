#include "Ship.h"

Ship::Ship(Vector2 pos, int length, Orientation orientation) : pos(pos), length(length), facing(orientation)
{
	for (int i = 0; i < length; ++i)
	{
		hitPoints.push_back(false);
	}

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

	footprint = Footprint(x, y, w, h); //pos still 0,0 for now but will be moved elsewhere
}

bool Ship::takeHit(Vector2 hit)
{
	int hitPlace = fromPointToLength(hit);

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
		case Destroyer:
			return 2;
		case Submarine:
		case Cruiser:
			return 3;
		case Battleship:
			return 4;
		case Carrier:
			return 5;
		case None:
		default:
			return -1;
	}
}
