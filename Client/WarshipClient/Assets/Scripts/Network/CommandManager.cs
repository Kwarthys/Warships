using System.Collections;
using System.Collections.Generic;
using System.Text;
using UnityEngine;

public class CommandManager : MonoBehaviour
{
    public enum CommandID { IDAttrib, RegisterOther, NameSend, TargetGrid, FireGrid, FireResult, ShipSunk, EndGame, PlaceShip, GameStarts }


    public void testSerialization()
    {
        //TEST
        StringCommand nameCommand = new StringCommand();
        nameCommand.id = CommandID.NameSend;
        nameCommand.param = 0;
        nameCommand.data = "Zykyflex";

        displayCommand(nameCommand);

        byte[] buf = new byte[255];

        int len = serializeCommand(nameCommand, buf);

        Command deserialized = deserializeCommand(buf, len);

        displayCommand(deserialized);

        IntArrayCommand intCommand = new IntArrayCommand();
        intCommand.id = CommandID.TargetGrid;
        intCommand.param = 1;
        intCommand.data =  new int[]{ 1,2,3,4,5,6,7,8,9};

        displayCommand(intCommand);

        len = serializeCommand(intCommand, buf);
        deserialized = deserializeCommand(buf, len);

        displayCommand(deserialized);


    }

    public Command deserializeCommand(byte[] rawCommand, int len)
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

            for (int i = 2; i < len; i++)
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

            int[] data = new int[len - 2];
            for (int i = 2; i < len; i++)
            {
                data[i - 2] = rawCommand[i];
            }

            command.data = data;

            return command;
        }

    }

    public int serializeCommand(Command command, byte[] buf)
    {
        if(command.id == CommandID.NameSend)
        {
            return serializeCommand((StringCommand)command, buf);
        }
        else
        {
            return serializeCommand((IntArrayCommand)command, buf);
        }
    }

    public int serializeCommand(IntArrayCommand command, byte[] buf)
    {
        buf[0] = (byte)command.id;
        buf[1] = (byte)command.param;

        int i;
        for (i = 0; i < command.data.Length; i++)
        {
            byte b;
            if(command.data[i] > 255)
            {
                Debug.LogWarning("Serializing int greater than 255 to 255.");
                b = 255;
            }
            else
            {
                b = (byte)command.data[i];
            }
            buf[i+2] = b;
        }

        return i+2;
    }

    public int serializeCommand(StringCommand command, byte[] buf)
    {
        buf[0] = (byte)command.id;
        buf[1] = (byte)command.param;

        int i;
        for (i = 0; i < command.data.Length; i++)
        {
            buf[i+2] = (byte)command.data[i];
        }

        return i + 2;
    }

    public void displayCommand(Command c)
    {
        if(c.id == CommandID.NameSend)
        {
            displayStringCommand((StringCommand)c);
        }
        else
        {
            displayIntArrayCommand((IntArrayCommand)c);
        }
    }

    public void displayStringCommand(StringCommand c)
    {
        Debug.Log(c.id + " " + c.param + ": " + c.data);
    }

    public void displayIntArrayCommand(IntArrayCommand c)
    {
        StringBuilder sb = new StringBuilder();
        sb.Append(c.id);
        sb.Append(" ");
        sb.Append(c.param);
        sb.Append(": ");

        for (int i = 0; i < c.data.Length; i++)
        {
            sb.Append(c.data[i]);
            sb.Append(" ");
        }

        Debug.Log(sb.ToString());
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
