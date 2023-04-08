using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Ship
{
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
}
