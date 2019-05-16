using System.Net;
using System.Reflection.Metadata.Ecma335;
using System.Threading.Tasks;

namespace ER_NET.Server
{
    public interface ICommunicationSender
    {
        Task SendMessageAsync(byte[] data, IPAddress ip, uint port);
    }
}