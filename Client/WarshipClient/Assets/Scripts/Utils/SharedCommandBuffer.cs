using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Threading;

public class SharedCommandBuffer
{
    private Mutex mutex = new Mutex();

    private Queue<Command> buffer = new Queue<Command>();

    public void waitToAdd(Command c)
    {
        mutex.WaitOne();
        buffer.Enqueue(c);
        mutex.ReleaseMutex();
    }

    public bool tryToGet(out Command c)
    {
        if(mutex.WaitOne(System.TimeSpan.FromMilliseconds(0.1))) //0.1ms wait on mainthread
        {
            //can read
            if (buffer.Count > 0)
            {
                c = buffer.Dequeue();
                int size = buffer.Count;
                mutex.ReleaseMutex();

                DebugTextManager.instance.sendTextToDebug("Successfully retrived command from buffer, " + size + " left.");
                return true;
            }

            mutex.ReleaseMutex();
        }

        //can't read or buffer empty
        c = null;
        return false;        
    }
}
