using System;
using System.Net;
using System.Net.Sockets;
using System.Threading.Tasks;
using ER_NET.Server;

namespace ER_NET.Shared
{
    public class TcpSender : ICommunicationSender
    {
        public async Task SendMessageAsync(byte[] data, IPAddress ip, uint port)
        {
                await Task.Run(() =>
                {
                    using (var tcpClient = new TcpClient())
                    {
                        try
                        {
                            if (tcpClient.ConnectAsync(ip.ToString(), (int)port).Wait(1000))
                            {
                                using (var stream = tcpClient.GetStream())
                                {
                                    stream.Write(data, 0, data.Length);
                                }
                            }
                        }
                        catch (Exception e)
                        {
                            Console.WriteLine(e);
                        }
                    }
                });
        }
    }
}