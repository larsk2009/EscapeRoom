using System;
using ER_NET.Server;

namespace ControlUnit
{
    class Program
    {
        static void Main(string[] args)
        {
            DiscoveryServer discoveryServer = new DiscoveryServer();
            discoveryServer.DoDiscovery();
        }
    }
}