using System;

namespace ER_NET.Shared
{
    public interface ICommunicationParser
    {
        void Start();
        event EventHandler<CommunicationEventArgs> OnCommunicationEvent;
    }
}