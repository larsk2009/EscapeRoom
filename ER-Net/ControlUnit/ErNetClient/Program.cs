using System;
using System.Threading.Tasks;
using ER_NET.Client;
using ER_NET.Shared;

namespace ErNetClient
{
    class Program
    {
        static void Main(string[] args)
        {
            var sender = UdpSender.Instance;
            var parser = new UdpParser();
            ER_NET.Client.ErNetClientEngine clientEngine = new ER_NET.Client.ErNetClientEngine("SoftwarePuzzle", sender, parser);

            string input;
            do
            {
                Console.Write("> ");
                input = Console.ReadLine();

                if (input == "DisplayNumber")
                {
                    Task.Run(async () =>
                    {
                        var displayNumber = await clientEngine.GetDisplayNumberAsync();
                        Console.WriteLine(displayNumber.ToString());
                        Console.Write("> ");
                    });
                } else if (input == "Solution")
                {
                    Task.Run(async () =>
                    {
                        var solution = await clientEngine.GetSolutionAsync();
                        Console.WriteLine(solution.ToString());
                        Console.Write("> ");
                    });
                }
            } while (input != "quit");
        }
    }
}