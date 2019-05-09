using System;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Threading.Tasks;

namespace ER_NET.Shared
{
    public class TcpParser : ICommunicationParser
    {
        private TcpListener _server;
        private const int DiscoveryPort = 46666;
        private const int ResponsePort = 46667;
        
        //TCP received event variables
        //public delegate void TcpEventHandler(object sender, CommunicationEventArgs e);

        public event EventHandler<CommunicationEventArgs> OnCommunicationEvent;
        
        public TcpParser()
        {
            _server = new TcpListener(IPAddress.Any, ResponsePort);
        }

        public void Start()
        {
            _server.Start();

            Task.Run(async () =>
            {
                while (true)
                {
                    var client = await _server.AcceptTcpClientAsync().ConfigureAwait(false);
                    NetworkStream stream = client.GetStream();
                    using (var streamReader = new StreamReader(stream))
                    {
                        string data = await streamReader.ReadToEndAsync();
                        OnMessageReceived(data, ((IPEndPoint) client.Client.RemoteEndPoint).Address);
                    }

                    client.Close();
                }
            });
        }

        public void OnMessageReceived(string json, IPAddress address)
        {
            var message = Message.FromJson(json);
            if (message != null)
            {
                RaiseTcpEvent(message, address);
            }
        }

        protected virtual void RaiseTcpEvent(Message message, IPAddress address)
        {
            OnCommunicationEvent?.Invoke(this, new CommunicationEventArgs(message, address));
        }
    }
}