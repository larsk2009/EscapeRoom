using System.Net;
using System.Reflection.Metadata.Ecma335;

namespace ER_NET.Server
{
    public interface ITcpSender
    {
        void SendMessage(byte[] data, IPAddress ip, uint port);
    }
}