using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SharedCommandBuffer
{
    private volatile bool theLock = false;

    private volatile List<Command> buffer = new List<Command>();

    public void waitToAdd(Command c)
    {
        while (theLock) ;
        theLock = true;
        buffer.Add(c);
        theLock = false;
    }

    public bool tryToGet(out Command c)
    {
        if(!theLock)
        {
            c = null;
            return false;
        }

        int size = 0;
        theLock = true;

        if(buffer.Count == 0)
        {
            c = null;
        }
        else
        {
            c = buffer[0];
            buffer.RemoveAt(0);
            size = buffer.Count;
        }

        theLock = false;
        Debug.Log("Successfully retrived command from buffer, " + size + " left.");
        return true;
    }
}
