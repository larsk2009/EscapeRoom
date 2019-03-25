using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;

namespace ER_NET.Server
{
    public class DiscoveryServer
    {
        private readonly UdpClient _client;
        private const int DiscoveryPort = 46666;
        private const int ResponsePort = 46667;

        private Guid _guid;

        public DiscoveryServer(Guid guid)
        {
            _guid = guid;
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
                Guid = _guid,
                MessageType = "Discovery"
            };
            SendUdpData(data);
        }

        /// <summary>
        /// Starts a continous discovery pattern. Can't be stopped
        /// </summary>
        public void ContinuousDiscovery()
        {
            //Run discovery every 3 seconds on a Task
            Task.Run(async () =>
            {
                while (true)
                {
                    DoDiscovery();
                    await Task.Delay(TimeSpan.FromSeconds(3));
                }
            });
        }

        private void SendUdpData(object data)
        {
            var json = "ER-NET\n" + JsonConvert.SerializeObject(data);
            var bytes = Encoding.ASCII.GetBytes(json);

            var broadCastEndPoint = new IPEndPoint(IPAddress.Broadcast, DiscoveryPort);

            _client.Send(bytes, bytes.Length, broadCastEndPoint);
        }
    }
}