using System;
using ER_NET.Shared;

namespace ER_NET_tests.Mocks
{
    public class MockTcpParser : ITcpParser
    {
        public bool _isStarted = false;
        public event EventHandler<TcpEventArgs> OnTcpEvent;

        public void Start()
        {
            _isStarted = true;
        }

        public void RaiseTcpEvent(TcpEventArgs eventArgs)
        {
            OnTcpEvent?.Invoke(this, eventArgs);
        }
    }
}