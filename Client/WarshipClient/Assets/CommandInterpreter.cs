using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CommandInterpreter
{
    public void treatIncomingCommand(Command c)
    {
        if(c.id == CommandManager.CommandID.GameStarts)
        {
            GameManager.instance.gameStarts();
        }
        else if(c.id == CommandManager.CommandID.NameSend)
        {
            StringCommand command = (StringCommand)c;
            GameManager.instance.registerPlayerName(command.param, command.data);
        }
        else
        {
            IntArrayCommand command = (IntArrayCommand)c;

            switch(c.id)
            {
                case CommandManager.CommandID.IDAttrib:
                    GameManager.instance.registerLocalPlayerID(c.param, command.data[0]);
                    break;
                case CommandManager.CommandID.RegisterOther:
                    GameManager.instance.registerPlayer(c.param, command.data[0]);
                    break;

                case CommandManager.CommandID.EndGame:
                    Debug.LogWarning(c.id + " Not Yet Implemented");
                    break;

                case CommandManager.CommandID.FireGrid:
                    Debug.LogWarning(c.id + " Not Yet Implemented");
                    break;

                case CommandManager.CommandID.FireResult:
                    Debug.LogWarning(c.id + " Not Yet Implemented");
                    break;

                case CommandManager.CommandID.TargetGrid:
                    Debug.LogWarning(c.id + " Not Yet Implemented");
                    break;

                case CommandManager.CommandID.ShipSunk:
                    Debug.LogWarning(c.id + " Not Yet Implemented");
                    break;
            }
        }
    }
}
