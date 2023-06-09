using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CommandInterpreter
{
    public void treatIncomingCommand(Command c)
    {
        bool printToDebug = true;
        if(printToDebug)
        {
            DebugTextManager.instance.sendTextToDebug(CommandManager.displayCommand(c) + "\n");
        }

        if(c.id == CommandManager.CommandID.NameSend)
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

                case CommandManager.CommandID.GameStarts:
                    GameManager.instance.gameStarts(c.param, command.data[0] == 1);
                    break;

                case CommandManager.CommandID.EndGame:
                    Debug.LogWarning(c.id + " Not Yet Implemented");
                    break;

                case CommandManager.CommandID.FireGrid:
                    GameManager.instance.ownFireResults(c.param, command.data);
                    break;

                case CommandManager.CommandID.FireReady:
                    GameManager.instance.fireReady(c.param, command.data[0] == 1);
                    break;

                case CommandManager.CommandID.FireResult:
                    GameManager.instance.manageFireResults(c.param, command.data);
                    break;

                case CommandManager.CommandID.TargetGrid:
                    GameManager.instance.registerOpponentTargeting(c.param, command.data[0], command.data[1]);
                    break;

                case CommandManager.CommandID.ShipSunk:
                    GameManager.instance.newShipScore(c.param, command.data);
                    break;
            }
        }
    }
}
