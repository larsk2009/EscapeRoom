using System;
using System.ComponentModel;
using System.Threading.Tasks;
using ER_NET.Shared;

namespace ER_NET.Server
{
    public class ErNetServerEngine
    {
        public ErNetServerEngine()
        {
            var guid = Guid.NewGuid();
            var discoveryServer = new DiscoveryServer(guid);
            discoveryServer.ContinuousDiscovery();
            
            var parser = new TcpParser();
            parser.Start();
            parser.OnTcpEvent += (sender, eventArgs) =>
            {
                Console.WriteLine(
                    $"Received MessageType {eventArgs.Message.MessageType} from GUID {eventArgs.Message.Id} with IP {eventArgs.RemoteIp}");
            };

        }
    }
}