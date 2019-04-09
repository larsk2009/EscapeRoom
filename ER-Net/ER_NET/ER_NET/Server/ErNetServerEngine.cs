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
        private ITcpParser _parser;
        private IDiscoveryServer _discoveryServer;
        private ITcpSender _tcpSender;

        public ErNetServerEngine(ITcpParser parser, IDiscoveryServer discoveryServer, ITcpSender tcpSender)
        {
            _parser = parser;
            _discoveryServer = discoveryServer;
            _tcpSender = tcpSender;
            var guid = Guid.NewGuid();
            _discoveryServer.guid = guid;
            _discoveryServer.ContinuousDiscovery();

            Task.Run(async () =>
            {
                while (true)
                {
                    CheckConnectedDevices();
                    await Task.Delay(3);
                }
            });

            _parser.Start();
            _parser.OnTcpEvent += (sender, eventArgs) =>
            {
                var message = eventArgs.Message;
                if (message == null)
                {
                    return;
                }
                Console.WriteLine(
                    $"Received MessageType {message.MessageType} from GUID {message.Id} with IP {eventArgs.RemoteIp}");
                if (message.MessageType == "DiscoveryAcknowledge")
                {
                    if (!_connectedDevices.ContainsKey(message.Id))
                    {
                        _connectedDevices.Add(message.Id, new Device(message.Id, eventArgs.RemoteIp));
                    }
                    else
                    {
                        _connectedDevices[message.Id].LastConnected = DateTime.UtcNow;
                    }

                    _discoveryServer.DiscoveryAcknowledgeReceived(message, eventArgs.RemoteIp);
                } else if (message.MessageType == "GetDisplayNumber")
                {
                    if (_connectedDevices.ContainsKey(message.Id))
                    {
                        var displayNumber = _connectedDevices[message.Id].DisplayNumber;
                        
                        var response = new Message
                        {
                            Id = guid,
                            MessageType = "DisplayNumber",
                            Value = displayNumber.ToString()
                        };
                        _tcpSender.SendMessage(response.ToBytes(), eventArgs.RemoteIp, CommunicationPorts.ResponsePort);
                    }
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

                //Console.WriteLine("Device ID: {0} with IP {1} last seen at {2}", device.Value.Id, device.Value.IpAddress, device.Value.LastConnected);
            }

            //After enumerating over the whole list, remove the devices
            foreach (var guid in removeList)
            {
                _connectedDevices.Remove(guid);
            }
        }
    }
}