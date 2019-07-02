using System.Diagnostics;
using System.Threading.Tasks;
using ER_NET.Client;
using jumpPuzzle.Hubs;
using jumpPuzzle.Models;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.SignalR;

namespace jumpPuzzle.Controllers
{
    public class HomeController : Controller
    {
        private readonly IHubContext<ResetDevicesHub> _hubContext;

        public HomeController(IHubContext<ResetDevicesHub> hubContext)
        {
            _hubContext = hubContext;

            ErNetClientEngine.Instance.OnReset += async (sender, args) =>
            {
                await _hubContext.Clients.All.SendAsync("ResetReceived");
            };
        }

        public IActionResult Index()
        {
            return View();
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

        public async Task<IActionResult> GetDisplayNumber()
        {
            if (ErNetClientEngine.Instance != null)
            {
                var instance = ErNetClientEngine.Instance;
                var displayNumber = await instance.GetDisplayNumber();

                return Ok(displayNumber);
            }

            return BadRequest();
        }
    }
}