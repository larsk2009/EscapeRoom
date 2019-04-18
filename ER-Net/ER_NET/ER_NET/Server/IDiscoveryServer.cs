using System;
using System.Net;
using ER_NET.Shared;

namespace ER_NET.Server
{
    public interface IDiscoveryServer
    {
        Guid guid { get; set; }

        void ContinuousDiscovery();
        void DiscoveryAcknowledgeReceived(Message message, IPAddress ipAddress);
    }
}