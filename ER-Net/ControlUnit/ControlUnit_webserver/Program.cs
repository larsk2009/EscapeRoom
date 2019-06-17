using System;
using System.Runtime.InteropServices;
using System.Threading;
using Microsoft.AspNetCore;
using Microsoft.AspNetCore.Hosting;
using Microsoft.Extensions.PlatformAbstractions;
using UtilityDelta.Gpio.Implementation;

namespace ControlUnit_webserver
{
    public class Program
    {
        public static void Main(string[] args)
        {
            using (var controller = new PinController(new FileIo(), new SysfsPinMapper()))
            {
                var pin = controller.GetGpioPin("PA12");

                int lightTimeInMilliseconds = 1000;
                int dimTimeInMilliseconds = 200;

                while (true)
                {
                    Console.WriteLine($"Light for {lightTimeInMilliseconds}ms");
                    pin.PinValue = true;
                    Thread.Sleep(lightTimeInMilliseconds);
                    Console.WriteLine($"Dim for {dimTimeInMilliseconds}ms");
                    pin.PinValue = false;
                    Thread.Sleep(dimTimeInMilliseconds);
                }
            }
            
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