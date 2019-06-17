using ER_NET.Client;
using ER_NET.Shared;
using Microsoft.AspNetCore;
using Microsoft.AspNetCore.Hosting;

namespace jumpPuzzle
{
    public class Program
    {
        public static void Main(string[] args)
        {
            var engine = new ErNetClientEngine("Software Puzzle", UdpSender.Instance, new UdpParser());

            CreateWebHostBuilder(args).Build().Run();
        }

        public static IWebHostBuilder CreateWebHostBuilder(string[] args) =>
            WebHost.CreateDefaultBuilder(args)
                .UseStartup<Startup>();
    }
}