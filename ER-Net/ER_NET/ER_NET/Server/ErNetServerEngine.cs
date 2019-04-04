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
        private Dictionary<Guid, Device> _connectedDevices = new Dictionary<Guid, Device>();

        public ErNetServerEngine(ITcpParser parser)
        {
            var guid = Guid.NewGuid();
            var discoveryServer = new DiscoveryServer(guid);
            discoveryServer.ContinuousDiscovery();

            Task.Run(async () =>
            {
                while (true)
                {
                    CheckConnectedDevices();
                    await Task.Delay(3);
                }
            });

            parser.Start();
            parser.OnTcpEvent += (sender, eventArgs) =>
            {
                var message = eventArgs.Message;
                if (message == null)
                {
                    return;
                }
                Console.WriteLine(
                    $"Received MessageType {message.MessageType} from GUID {message.Id} with IP {eventArgs.RemoteIp}");
                if (eventArgs.Message.MessageType == "DiscoveryAcknowledge")
                {
                    if (!_connectedDevices.ContainsKey(message.Id))
                    {
                        _connectedDevices.Add(message.Id, new Device(message.Id, eventArgs.RemoteIp));
                        ;
                    }
                    else
                    {
                        _connectedDevices[message.Id].LastConnected = DateTime.UtcNow;
                    }

                    discoveryServer.DiscoveryAcknowledgeReceived(message, eventArgs.RemoteIp);
                }
            };
        }

        public bool IsDeviceConnected(Guid guid)
        {
            return _connectedDevices.ContainsKey(guid);
        }
        
        private void CheckConnectedDevices()
        {
            var now = DateTime.UtcNow;
            var removeList = new List<Guid>();
            foreach (var device in _connectedDevices)
            {
                var time = device.Value.LastConnected;
                var difference = now - time;
                if (difference.Seconds > 10)
                {
                    removeList.Add(device.Key);
                    //We don't remove the device from the dictionary here because that will modify the collection and trow an exception
                }

                Console.WriteLine("Device ID: {0} with IP {1} last seen at {2}", device.Value.Id,
                    device.Value.IpAddress, device.Value.LastConnected);
            }

            //After enumerating over the whole list, remove the devices
            foreach (var guid in removeList)
            {
                _connectedDevices.Remove(guid);
            }
        }
    }
}