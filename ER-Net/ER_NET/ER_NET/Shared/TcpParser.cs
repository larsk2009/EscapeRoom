using System;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Threading.Tasks;

namespace ER_NET.Shared
{
    public class TcpParser
    {
        private TcpListener _server;
        private const int DiscoveryPort = 46666;
        private const int ResponsePort = 46667;
        
        //TCP received event variables
        //public delegate void TcpEventHandler(object sender, TcpEventArgs e);

        public event EventHandler<TcpEventArgs> OnTcpEvent;
        
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
                    var client = await _server.AcceptTcpClientAsync();
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
            OnTcpEvent?.Invoke(this, new TcpEventArgs(message, address));
        }
    }

    public class TcpEventArgs : EventArgs
    {
        public TcpEventArgs(Message message, IPAddress remoteIp)
        {
            Message = message;
            RemoteIp = remoteIp;
        }
        public Message Message { get; }
        public IPAddress RemoteIp { get; }
    }
}