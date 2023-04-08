using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GridNode
{
    public static int WATER = -1;

    public int shipIndex;
    public bool hit;

    public int nodeIndex;
    public Vector2Int coords;

    public GridNode(int nodeIndex, Vector2Int coords)
    {
        shipIndex = WATER;
        hit = false;

        this.nodeIndex = nodeIndex;
        this.coords = coords;
    }
}
