using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Ship
{
    public enum Orientation { NORTH, WEST, SOUTH, EAST};

    public Orientation orientation = Orientation.NORTH;
    public string shipName = "Ship";

    public int length { get; private set; }
    public int index { get; private set; }

    private bool[] hitPool;

    public Ship(int length, int index)
    {
        this.length = length;
        this.index = index;

        hitPool = new bool[length];
        for (int i = 0; i < hitPool.Length; i++)
        {
            hitPool[i] = false;
        }
    }

    public void rotate()
    {
        orientation = (Orientation)(((int)orientation + 1)%4); //that's a crappy looking line
    }

    public Ship(ShipScriptableObject shipScriptable, int index)
    {
        this.length = shipScriptable.length;
        this.index = index;

        this.shipName = shipScriptable.shipName;
    }

    public Vector2Int getShipRootOffset()
    {
        //TODO not sure of when to apply offset and offset sign
        int x = 0;
        int y = 0;

        int evenOffset = (length+1) % 2;//0 if len odd, 1 if len even

        switch(orientation)
        {
            case Orientation.NORTH:
                y = -(length/2);
                break;
            case Orientation.SOUTH:
                y = (length / 2) - evenOffset;
                break;
            case Orientation.EAST:
                x = -(length / 2);
                break;
            case Orientation.WEST:
                x = (length / 2) - evenOffset;
                break;
        }

        return new Vector2Int(x, y);
    }

    public RectInt computeShipFootprint(Vector2Int pos)
    {
        int x = pos.x;
        int y = pos.y;
        int w = 1;
        int h = 1;

        //Debug.Log("Pos:" + pos + " L:" + length + " O:" + orientation);

        if(orientation == Orientation.NORTH || orientation == Orientation.SOUTH)
        {
            h = length;
            y -= (length - 1) / 2;
        }
        else
        {
            w = length;
            x -= (length - 1) / 2;
        }

        //Debug.Log("x:" + x + " y:" + y + " w:" + w + " h:" + h);

        return new RectInt(x, y, w, h);
    }
}
