using System;
using ER_NET.Shared;

namespace ER_NET_tests.Mocks
{
    public class MockCommunicationParser : ICommunicationParser
    {
        public bool _isStarted = false;
        public event EventHandler<CommunicationEventArgs> OnCommunicationEvent;

        public void Start()
        {
            _isStarted = true;
        }

        public void RaiseTcpEvent(CommunicationEventArgs eventArgs)
        {
            OnCommunicationEvent?.Invoke(this, eventArgs);
        }
    }
}