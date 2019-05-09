using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace ER_NET.Shared
{
    public class UdpParser : ICommunicationParser
    {
        public void Start()
        {
            Task.Run(async () =>
            {
                using (var client = new UdpClient((int)CommunicationPorts.CommunicationPort))
                {
                    while (true)
                    {
                        var receivedResults = await client.ReceiveAsync().ConfigureAwait(false);
                        var jsonString = Encoding.ASCII.GetString(receivedResults.Buffer);
                        
                        var message = Message.FromJson(jsonString);
                        
                        RaiseCommunicationEvent(message, receivedResults.RemoteEndPoint.Address);
                    }
                }
            });
        }

        public event EventHandler<CommunicationEventArgs> OnCommunicationEvent;
        
        protected virtual void RaiseCommunicationEvent(Message message, IPAddress address)
        {
            OnCommunicationEvent?.Invoke(this, new CommunicationEventArgs(message, address));
        }
    }
}