using System.Net;
using System.Threading.Tasks;

namespace ER_NET.Server
{
    public interface ICommunicationSender
    {
        Task SendMessageAsync(byte[] data, IPAddress ip, uint port);
    }
}