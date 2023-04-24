using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CommandSender
{
    private void sendCommand(Command command)
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

    public void sendFireCommand(int targetedGridIndex, int[] targetedNodeIndecies)
    {
        IntArrayCommand command = new IntArrayCommand();
        command.id = CommandManager.CommandID.FireGrid;
        command.param = targetedGridIndex;
        command.data = targetedNodeIndecies;

        sendCommand(command);
    }

    public void sendPlaceShip(int shipOrientation, int rooNodeId)
    {
        IntArrayCommand command = new IntArrayCommand();
        command.id = CommandManager.CommandID.PlaceShip;
        command.param = shipOrientation;
        command.data = new int[1] { rooNodeId };

        sendCommand(command);
    }
}