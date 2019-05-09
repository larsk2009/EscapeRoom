using System;
using System.Reflection.PortableExecutable;
using System.Threading;

namespace ER_NET.Shared
{
    public interface ICommunicationParser
    {
        void Start();
        event EventHandler<CommunicationEventArgs> OnCommunicationEvent;
    }
}