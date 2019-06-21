using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using ER_NET.Shared;
using Newtonsoft.Json;

namespace ER_NET.Server
{
    public class DiscoveryServer : IDiscoveryServer
    {
        private readonly UdpClient _client;

        public Guid guid
        {
            get => _guid;
            set => _guid = value;
        }
        private Guid _guid;

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
            var data = new Message
            {
                Id = _guid,
                MessageType =  "Discovery"
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

        public async void DiscoveryAcknowledgeReceived(Message message, IPAddress ipAddress)
        {
            if (message.MessageType == "DiscoveryAcknowledge")
            {
                var responseMessage = new Message
                {
                    Id = _guid,
                    MessageType = "Mute"
                };
                var data = responseMessage.ToBytes();

                await Task.Run(() =>
                {
                    using (var tcpClient = new TcpClient())
                    {
                        try
                        {
                            if (tcpClient.ConnectAsync(ipAddress.ToString(), (int)CommunicationPorts.ResponsePort).Wait(1000))
                            {
                                using (var stream = tcpClient.GetStream())
                                {
                                    stream.Write(data, 0, data.Length);
                                }
                            }
                        }
                        catch (Exception e)
                        {
                            Console.WriteLine(e);
                        }
                    }
                });
            }
        }

        private void SendUdpData(object data)
        {
            var json = "ER-NET\n" + JsonConvert.SerializeObject(data);
            var bytes = Encoding.ASCII.GetBytes(json);

            var broadCastEndPoint = new IPEndPoint(IPAddress.Broadcast, (int)CommunicationPorts.DiscoveryPort);

            _client.Send(bytes, bytes.Length, broadCastEndPoint);
        }
    }
}