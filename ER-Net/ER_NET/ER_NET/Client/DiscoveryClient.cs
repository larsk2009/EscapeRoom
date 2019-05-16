using System;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using ER_NET.Shared;

namespace ER_NET.Client
{
    public static class DiscoveryClient
    {
        private static string _name;

        public static bool IsMuted { get; set; } = false;

        public static void Start(string name)
        {
            _name = name;
            Console.Write("Listening on port ");
            Console.WriteLine(CommunicationPorts.CommunicationPort);

            Task.Run(async () =>
            {
                using (var udpClient = new UdpClient((int) CommunicationPorts.CommunicationPort))
                {
                    while (true)
                    {
                        var receivedResults = await udpClient.ReceiveAsync();
                        var jsonString = Encoding.ASCII.GetString(receivedResults.Buffer);
                        Console.WriteLine(jsonString);
                        var message = Message.FromJson(jsonString);
                        if (message.MessageType == "Discovery" && !IsMuted)
                        {
                            //Return message
                            Console.WriteLine("respond");
                            var address = receivedResults.RemoteEndPoint.Address;
                            var responseMessage = new Message
                            {
                                Name = _name,
                                MessageType = "DiscoveryAcknowledge"
                            };
                            var data = Encoding.ASCII.GetBytes(responseMessage.ToJsonString());
                            using (var tcpClient = new TcpClient())
                            {
                                //Try to connect to the control unit. 
                                if (tcpClient.ConnectAsync(address.ToString(), (int)CommunicationPorts.CommunicationPort).Wait(1000))
                                {
                                    //This only happens when the connection was established with the control unit
                                    using (var stream = tcpClient.GetStream())
                                    {
                                        stream.Write(data, 0, data.Length);
                                    }
                                }
                            }
                        }
                    }
                }
            });
        }
    }
}