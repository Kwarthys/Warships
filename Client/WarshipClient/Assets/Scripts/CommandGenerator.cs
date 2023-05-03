using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CommandGenerator : MonoBehaviour
{
    private CommandSender commandSender = new CommandSender();

    public CommandManager.CommandID commandID;
    public int param;
    public int[] intArrayData;
    public string stringData;

    public bool sendToServer = false;

    // Update is called once per frame
    void Update()
    {
        if(sendToServer)
        {
            sendToServer = false;

            Command c;

            if(commandID == CommandManager.CommandID.NameSend)
            {
                StringCommand sc = new StringCommand();
                sc.data = stringData;

                c = sc;
            }
            else
            {
                IntArrayCommand ic = new IntArrayCommand();
                ic.data = intArrayData;

                c = ic;
            }

            c.id = commandID;
            c.param = param;

            commandSender.sendCommand(c);
        }
    }
}
