using System.Net;
using System.Threading.Tasks;
using ER_NET.Server;
using Xunit.Sdk;

namespace ER_NET_tests.Mocks
{
    public class MockCommunicationSender : ICommunicationSender
    {
        public byte[] lastMessageSent;
        public IPAddress lastIp;
        public uint lastPort;
        
        public bool IsMessageSend = false;
#pragma warning disable 1998
        public async Task SendMessageAsync(byte[] data, IPAddress ip, uint port)
#pragma warning restore 1998
        {
            lastMessageSent = data;
            lastIp = ip;
            lastPort = port;
        }
    }
}