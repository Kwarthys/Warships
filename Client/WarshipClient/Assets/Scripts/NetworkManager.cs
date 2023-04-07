using System.Collections;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;
using UnityEngine;

public class NetworkManager : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        test();
    }


    public void test()
    {
        /*
        IPHostEntry ipHostInfo = await Dns.GetHostEntryAsync("host.contoso.com");
        IPAddress ipAddress = ipHostInfo.AddressList[0];
        IPEndPoint ipEndPoint = new(ipAddress, 11_000);
        */


        //connect
        using Socket socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

        socket.Connect("127.0.0.1", 4242);
        Debug.Log("TCP Connected");

        byte[] rawData = new byte[1024];
        socket.Receive(rawData);
        string data = Encoding.Default.GetString(rawData);

        Debug.Log("Received " + data);

        socket.Shutdown(SocketShutdown.Both);
    }
}
