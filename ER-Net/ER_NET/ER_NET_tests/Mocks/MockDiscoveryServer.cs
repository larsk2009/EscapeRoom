using System;
using System.Net;
using ER_NET.Server;
using ER_NET.Shared;

namespace ER_NET_tests.Mocks
{
    public class MockDiscoveryServer : IDiscoveryServer
    {
        public string Name { get; set; }
        public bool IsDiscovery = false;
        public bool IsDiscoveryAck = false;
        public void ContinuousDiscovery()
        {
            IsDiscovery = true;
        }

        public void DiscoveryAcknowledgeReceived(Message message, IPAddress ipAddress)
        {
            IsDiscoveryAck = true;
        }
    }
}