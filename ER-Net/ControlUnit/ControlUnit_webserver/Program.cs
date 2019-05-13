using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Threading.Tasks;
using Microsoft.AspNetCore;
using Microsoft.AspNetCore.Hosting;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Logging;
using Microsoft.Extensions.PlatformAbstractions;

namespace ControlUnit_webserver
{
    public class Program
    {
        public static void Main(string[] args)
        {
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