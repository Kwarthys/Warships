using System.Collections;
using System.Collections.Generic;
using System.Text;
using UnityEngine;

public class CommandManager : MonoBehaviour
{
    public enum CommandID { IDAttrib, NameSend, TargetGrid, FireGrid, FireResult, ShipSunk, EndGame }

    private void Start()
    {
        //TEST
        StringCommand nameCommand = new StringCommand();
        nameCommand.id = CommandID.NameSend;
        nameCommand.param = 0;
        nameCommand.data = "Zykyflex";

        displayStringCommand(nameCommand);

        byte[] c = serializeCommand(nameCommand);

        Command deserialized = deserializeCommand(c);

        displayStringCommand((StringCommand)deserialized);


    }

    public Command deserializeCommand(byte[] rawCommand)
    {
        //Command Code
        CommandID cid = (CommandID)rawCommand[0];
        int parameter = rawCommand[1];

        if(cid == CommandID.NameSend)
        {
            //String command
            StringCommand command = new StringCommand();
            command.id = cid;
            command.param = parameter;

            StringBuilder builder = new StringBuilder();

            for (int i = 2; i < rawCommand.Length; i++)
            {
                builder.Append((char)rawCommand[i]);
            }

            command.data = builder.ToString();

            return command;
        }
        else
        {
            //int[] command
            IntArrayCommand command = new IntArrayCommand();
            command.id = cid;
            command.param = parameter;

            int[] data = new int[rawCommand.Length - 2];
            for (int i = 2; i < rawCommand.Length; i++)
            {
                data[i - 2] = rawCommand[i];
            }

            command.data = data;

            return command;
        }

    }

    public byte[] serializeCommand(StringCommand command)
    {
        List<byte> serialized = new List<byte>();

        serialized.Add((byte)command.id);
        serialized.Add((byte)command.param);

        for (int i = 0; i < command.data.Length; i++)
        {
            serialized.Add((byte)command.data[i]);
        }

        return serialized.ToArray();
    }

    public void displayStringCommand(StringCommand c)
    {
        Debug.Log(c.id + " " + c.param + ": " + c.data);
    }
}

public class Command
{
    public CommandManager.CommandID id;
    public int param;
}

public class StringCommand : Command
{
    public string data;
}

public class IntArrayCommand : Command
{
    public int[] data;
}
