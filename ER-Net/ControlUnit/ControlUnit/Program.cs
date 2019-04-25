using System;
using System.Diagnostics.Tracing;
using System.Threading.Tasks;
using ER_NET.Server;
using ER_NET.Shared;

namespace ControlUnit
{
    class Program
    {
        static void Main(string[] args)
        {
            var parser = new TcpParser();
            var server = new DiscoveryServer();
            var tcpSender = new TcpSender();
            var engine = new ErNetServerEngine(parser, server, tcpSender);

            string input;
            do
            {
                input = Console.ReadLine();
                switch (input)
                {
                    case "reset":
                        engine.ResetDevices();
                        break;
                }
            } while (input != "quit");
        }
    }
}