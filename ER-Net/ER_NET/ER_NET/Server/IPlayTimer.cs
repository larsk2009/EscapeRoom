using System;
using System.Diagnostics.Tracing;

namespace ER_NET.Server
{
    public interface IPlayTimer
    {
        void Start();
        void Stop();

        event EventHandler<EventArgs> OnTick;
    }
}