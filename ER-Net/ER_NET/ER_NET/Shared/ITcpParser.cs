using System;
using System.Reflection.PortableExecutable;
using System.Threading;

namespace ER_NET.Shared
{
    public interface ITcpParser
    {
        void Start();
        event EventHandler<TcpEventArgs> OnTcpEvent;
    }
}