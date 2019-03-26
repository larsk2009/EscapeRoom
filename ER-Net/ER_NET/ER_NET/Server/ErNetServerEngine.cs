using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Net;
using System.Threading.Tasks;
using ER_NET.Shared;

namespace ER_NET.Server
{
    public class ErNetServerEngine
    {
        private Dictionary<Guid, IPAddress> _connectedDevices = new Dictionary<Guid, IPAddress>();
        
        public ErNetServerEngine()
        {
            var guid = Guid.NewGuid();
            var discoveryServer = new DiscoveryServer(guid);
            discoveryServer.ContinuousDiscovery();
            
            var parser = new TcpParser();
            parser.Start();
            parser.OnTcpEvent += (sender, eventArgs) =>
            {
                var message = eventArgs.Message;
                Console.WriteLine(
                    $"Received MessageType {message.MessageType} from GUID {message.Id} with IP {eventArgs.RemoteIp}");
                if (eventArgs.Message.MessageType == "DiscoveryAcknowledge")
                {
                    _connectedDevices.Add(message.Id, eventArgs.RemoteIp);
                    discoveryServer.DiscoveryAcknowledgeReceived(message, eventArgs.RemoteIp);
                }
            };

        }
    }
}