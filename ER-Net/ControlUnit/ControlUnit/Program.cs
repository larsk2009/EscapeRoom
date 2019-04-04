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
            var engine = new ErNetServerEngine(parser);
            Console.ReadKey(); //Wait for user input before closing
        }
    }
}