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
            var parser = new UdpParser();
            var server = new DiscoveryServer(UdpSender.Instance);
            var udpSender = UdpSender.Instance;
            var engine = new ErNetServerEngine(parser, server, udpSender);

            Console.WriteLine("Control Unit started");

            string input;
            do
            {
                Console.Write("> ");
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