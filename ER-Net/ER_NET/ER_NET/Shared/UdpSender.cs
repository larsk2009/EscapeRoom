using System.Net;
using System.Net.Sockets;
using System.Threading.Tasks;
using ER_NET.Server;

namespace ER_NET.Shared
{
    public class UdpSender : ICommunicationSender
    {
        public static UdpSender Instance
        {
            get
            {
                if (_instance == null)
                {
                    _instance = new UdpSender();
                }

                return _instance;
            }
        }

        private static UdpSender _instance = null;

        private UdpSender()
        {
        }

        public async Task SendMessageAsync(byte[] data, IPAddress ip, uint port)
        {
            using (var udpClient = new UdpClient())
            {
                udpClient.EnableBroadcast = true;
                udpClient.MulticastLoopback = true;
                await udpClient.SendAsync(data, data.Length, new IPEndPoint(ip, (int) CommunicationPorts.CommunicationPort));
            }
        }
    }
}