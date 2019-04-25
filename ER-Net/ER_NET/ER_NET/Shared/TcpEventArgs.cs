using System;
using System.Net;

namespace ER_NET.Shared
{
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