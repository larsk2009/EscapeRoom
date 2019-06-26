using System.Runtime.InteropServices;
using ER_NET.Client;
using ER_NET.Shared;
using Microsoft.AspNetCore;
using Microsoft.AspNetCore.Hosting;
using Microsoft.Extensions.PlatformAbstractions;

namespace jumpPuzzle
{
    public class Program
    {
        public static void Main(string[] args)
        {
            var engine = new ErNetClientEngine("SoftwarePuzzle", UdpSender.Instance, new UdpParser());

            CreateWebHostBuilder(args).Build().Run();
        }

        public static IWebHostBuilder CreateWebHostBuilder(string[] args)
        {
            if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
            {
                return WebHost.CreateDefaultBuilder(args)
                    .UseKestrel()
                    .UseStartup<Startup>();
            }
            else
            {
                return WebHost.CreateDefaultBuilder(args)
                    .UseKestrel()
                    .UseContentRoot(PlatformServices.Default.Application.ApplicationBasePath)
                    .UseStartup<Startup>();
            }
        }
    }
}