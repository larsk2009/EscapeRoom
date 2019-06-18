using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;
using ControlUnit_webserver.Hubs;
using Microsoft.AspNetCore.Mvc;
using ControlUnit_webserver.Models;
using ER_NET.Server;
using Microsoft.AspNetCore.SignalR;

namespace ControlUnit_webserver.Controllers
{
    public class HomeController : Controller
    {
        private readonly IHubContext<ConnectedDevicesHub> _hubContext;

        public HomeController(IHubContext<ConnectedDevicesHub> hubContext)
        {
            _hubContext = hubContext;
            
            ErNetServerEngine.Instance.OnDevicesListChanged += (sender, args) =>
            {
                _hubContext.Clients.All.SendAsync("AddDevice", ErNetServerEngine.Instance.Devices);
            };

            ErNetServerEngine.Instance.OnReset += (sender, args) =>
            {
                _hubContext.Clients.All.SendAsync("SolutionChanged", ErNetServerEngine.Instance.Solution);
                _hubContext.Clients.All.SendAsync("AddDevice", ErNetServerEngine.Instance.Devices);
            };

            ErNetServerEngine.Instance.OnTimerTick += (sender, args) =>
            {
                _hubContext.Clients.All.SendAsync("TimerUpdate", ErNetServerEngine.Instance.TimeLeft);
            };

            ErNetServerEngine.Instance.OnStatusChanged += (sender, args) =>
            {
                _hubContext.Clients.All.SendAsync("StatusChanged", ErNetServerEngine.Instance.Status);
            };
        }
        
        public IActionResult Index()
        {
            var viewModel = new ControlPageViewModel
            {
                Status = ErNetServerEngine.Instance.Status,
                PlayTime = ErNetServerEngine.Instance.TimeLeft,
                Devices = ErNetServerEngine.Instance.Devices,
                Solution = ErNetServerEngine.Instance.Solution
            };
            return View(viewModel);
        }

        public IActionResult About()
        {
            ViewData["Message"] = "Your application description page.";

            return View();
        }

        public IActionResult Contact()
        {
            ViewData["Message"] = "Your contact page.";

            return View();
        }

        public IActionResult Privacy()
        {
            return View();
        }

        [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
        public IActionResult Error()
        {
            return View(new ErrorViewModel {RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier});
        }
    }
}