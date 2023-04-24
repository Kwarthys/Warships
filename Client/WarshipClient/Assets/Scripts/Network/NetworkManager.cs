using System.Collections;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using UnityEngine;

public class NetworkManager : MonoBehaviour
{
    public static NetworkManager instance;

    [SerializeField]
    private bool debugConsoleDisplay = false;

    private void Awake()
    {
        instance = this;
    }

    private Socket socket;

    public CommandManager commandManager { get; private set; }
    private CommandInterpreter commandInterpreter = new CommandInterpreter();

    public SharedCommandBuffer commandBuffer { get; private set; } = new SharedCommandBuffer();

    private Thread tcpReader;

    void Start()
    {
        commandManager = GetComponent<CommandManager>();
        //test();

        startTCP();
    }

    private void Update()
    {
        if(commandBuffer.tryToGet(out Command c))
        {
            commandInterpreter.treatIncomingCommand(c);
        }
    }

    public void startTCP()
    {
        socket = setupTCPSocket();

        tcpReader = new Thread(infiniteReadFromServer);
        tcpReader.Start();
    }

    private Socket setupTCPSocket()
    {
        socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        socket.Connect("127.0.0.1", 4242);
        Debug.Log("TCP Connected");

        return socket;
    }


    public void test()
    {
        //connect
        socket = setupTCPSocket();

        byte[] rawData = new byte[1024];
        int len = socket.Receive(rawData);
        Command c = commandManager.deserializeCommand(rawData, len);
        commandManager.displayCommand(c);
    }

    public void send(byte[] buf, int len)
    {
        socket.Send(buf, len, SocketFlags.None);
    }

    private void OnApplicationQuit()
    {
        socket.Shutdown(SocketShutdown.Both);//we don't even say goodbye :(
        tcpReader.Interrupt();
    }

    //do not call on main thread
    private void infiniteReadFromServer()
    {
        while(true)
        {
            byte[] rawData = new byte[1024];
            int len = socket.Receive(rawData);
            if(len > 0)
            {
                Command c = commandManager.deserializeCommand(rawData, len);
                commandBuffer.waitToAdd(c);
#if UNITY_EDITOR
                if(debugConsoleDisplay)commandManager.displayCommand(c);
#endif
            }
        }
    }
}
