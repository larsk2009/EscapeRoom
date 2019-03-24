using System;

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