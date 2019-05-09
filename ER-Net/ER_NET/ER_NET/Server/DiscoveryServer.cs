using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using ER_NET.Shared;
using Newtonsoft.Json;

namespace ER_NET.Server
{
    public class DiscoveryServer : IDiscoveryServer
    {
        private readonly ICommunicationSender _sender;

        public String Name { get; set; }

        /*public Guid guid
        {
            get => _guid;
            set => _guid = value;
        }
        private Guid _guid;*/

        public DiscoveryServer(ICommunicationSender sender)
        {
            _sender = sender;
        }

        public async void DoDiscovery()
        {
            var data = new Message
            {
                //Id = _guid,
                Name = "ControlUnit",
                MessageType =  "Discovery"
            };
            await _sender.SendMessageAsync(data.ToBytes(), IPAddress.Broadcast, CommunicationPorts.CommunicationPort);
        }

        /// <summary>
        /// Starts a continous discovery pattern. Can't be stopped
        /// </summary>
        public void ContinuousDiscovery()
        {
            //Run discovery every 3 seconds on a Task
            Task.Run(async () =>
            {
                while (true)
                {
                    DoDiscovery();
                    await Task.Delay(TimeSpan.FromSeconds(3));
                }
            });
        }

        public async void DiscoveryAcknowledgeReceived(Message message, IPAddress ipAddress)
        {
            if (message.MessageType == "DiscoveryAcknowledge")
            {
                var responseMessage = new Message
                {
                    //Id = _guid,
                    Name = "ControlUnit",
                    MessageType = "Mute"
                };
                var data = responseMessage.ToBytes();

                //await _sender.SendMessageAsync(data, ipAddress, CommunicationPorts.CommunicationPort);
            }
        }
    }
}