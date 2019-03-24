using System;
using System.Net;
using System.Net.Sockets;

namespace PuzzleClient
{
    public class DiscoveryClient
    {
        private UdpClient _client;
        private const int DiscoveryPort = 49666;

        public DiscoveryClient()
        {
            _client = new UdpClient(DiscoveryPort)
            {
                EnableBroadcast = true,
                MulticastLoopback = true
            };

            _client.BeginReceive(new AsyncCallback(OnUdpData), _client);
            Console.Write("Listening on port ");
            Console.WriteLine(DiscoveryPort);
        }

        private void OnUdpData(IAsyncResult result)
        {
            //Gets the udp client. This is passed as the second variable in beginreceive
            UdpClient socket = result.AsyncState as UdpClient;
            IPEndPoint source = new IPEndPoint(0, 0); //This will store the ip address of the source
            byte[] message = socket?.EndReceive(result, ref source);


            //start listening again for new udp message
            socket.BeginReceive(new AsyncCallback(OnUdpData), socket);
        }
    }
}