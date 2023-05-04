using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CommandSender
{
    public void sendCommand(Command command)
    {
        byte[] byteBuffer = new byte[255];
        int len = NetworkManager.instance.commandManager.serializeCommand(command, byteBuffer);
        NetworkManager.instance.send(byteBuffer, len);
    }

    public void sendNameCommand(string name)
    {
        StringCommand command = new StringCommand();
        command.id = CommandManager.CommandID.NameSend;
        command.param = 0;//not used
        command.data = name;

        sendCommand(command);

    }

    public void sendTargetCommand(int targetedGridIndex, int targetedNodeIndex)
    {
        IntArrayCommand command = new IntArrayCommand();
        command.id = CommandManager.CommandID.TargetGrid;
        command.param = targetedGridIndex;
        command.data = new int[1] { targetedNodeIndex };

        sendCommand(command);
    }

    public void sendNoLongerFireReady()
    {
        IntArrayCommand command = new IntArrayCommand();
        command.id = CommandManager.CommandID.FireGrid;
        command.param = 0;
        command.data = new int[1]{0};
        sendCommand(command);
    }

    public void sendFireCommand(Vector2Int[] targetedPairs)
    {
        IntArrayCommand command = new IntArrayCommand();
        command.id = CommandManager.CommandID.FireGrid;
        command.param = 0;
        command.data = new int[targetedPairs.Length * 2];

        for (int i = 0; i < targetedPairs.Length; i++)
        {
            command.data[i * 2] = targetedPairs[i].x;
            command.data[i * 2 + 1] = targetedPairs[i].y;
        }

        sendCommand(command);
    }

    public void sendPlaceShip(int shipOrientation, int rooNodeId, int shipType)
    {
        IntArrayCommand command = new IntArrayCommand();
        command.id = CommandManager.CommandID.PlaceShip;
        command.param = (int)shipType;
        command.data = new int[2] { shipOrientation, rooNodeId };

        sendCommand(command);
    }

    public void sendReadyCommand(bool readyState)
    {
        IntArrayCommand command = new IntArrayCommand();
        command.id = CommandManager.CommandID.GameStarts;
        command.param = 0;
        command.data = new int[1] { readyState ? 1 : 0 };

        sendCommand(command);
    }
}
