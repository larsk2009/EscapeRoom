using System;
using System.Net;

namespace ER_NET.Shared
{
    public class CommunicationEventArgs : EventArgs
    {
        public CommunicationEventArgs(Message message, IPAddress remoteIp)
        {
            Message = message;
            RemoteIp = remoteIp;
        }
        public Message Message { get; }
        public IPAddress RemoteIp { get; }
    }
}