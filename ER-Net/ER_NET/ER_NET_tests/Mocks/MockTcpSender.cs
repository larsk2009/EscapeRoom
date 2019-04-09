using System.Net;
using ER_NET.Server;
using Xunit.Sdk;

namespace ER_NET_tests.Mocks
{
    public class MockTcpSender : ITcpSender
    {
        public byte[] lastMessageSent;
        public IPAddress lastIp;
        public uint lastPort;
        
        public bool IsMessageSend = false;
        public void SendMessage(byte[] data, IPAddress ip, uint port)
        {
            lastMessageSent = data;
            lastIp = ip;
            lastPort = port;
        }
    }
}