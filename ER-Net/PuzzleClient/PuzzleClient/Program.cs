using System;
using ER_NET.Client;

namespace PuzzleClient
{
    class Program
    {
        static void Main(string[] args)
        {
            Guid guid = Guid.NewGuid();
            DiscoveryClient.Start(guid);
            
            Console.ReadKey(); //Wait for user input to close
        }
    }
}