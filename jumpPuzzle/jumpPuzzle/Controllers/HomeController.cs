using System.Diagnostics;
using System.Threading.Tasks;
using ER_NET.Client;
using jumpPuzzle.Models;
using Microsoft.AspNetCore.Mvc;

namespace jumpPuzzle.Controllers
{
    public class HomeController : Controller
    {
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