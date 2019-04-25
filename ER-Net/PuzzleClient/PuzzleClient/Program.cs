using System;
using ER_NET.Client;
using ER_NET.Shared;

namespace PuzzleClient
{
    class Program
    {
        static void Main(string[] args)
        {
            Guid guid = Guid.NewGuid();
            DiscoveryClient.Start(guid);

            var parser = new TcpParser();
            parser.Start();
            parser.OnTcpEvent += (sender, eventArgs) =>
            {
                var message = eventArgs.Message;
                Console.WriteLine(
                    $"Received MessageType {message.MessageType} from GUID {message.Id} with IP {eventArgs.RemoteIp}");
            };
            Console.ReadKey(); //Wait for user input before closing
        }
    }
}