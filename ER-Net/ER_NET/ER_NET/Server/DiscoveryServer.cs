using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using Newtonsoft.Json;

namespace ER_NET.Server
{
    public class DiscoveryServer
    {
        private UdpClient _client;
        private const int DiscoveryPort = 49666;


        public DiscoveryServer()
        {
            _client = new UdpClient
            {
                EnableBroadcast = true,
                MulticastLoopback = true,
            };
        }

        public void DoDiscovery()
        {
            var data = new
            {
                Guid = Guid.NewGuid().ToString(),
                MessageType = "Discovery"
            };
            SendUdpData(data);
        }

        private void SendUdpData(object data)
        {
            var json = "ER-NET\n" + JsonConvert.SerializeObject(data);
            var bytes = Encoding.ASCII.GetBytes(json);

            IPEndPoint broadCastEndPoint = new IPEndPoint(IPAddress.Broadcast, DiscoveryPort);

            _client.Send(bytes, bytes.Length, broadCastEndPoint);
        }
    }
}