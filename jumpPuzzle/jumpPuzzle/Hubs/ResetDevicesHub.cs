using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.SignalR;

namespace jumpPuzzle.Hubs
{
    public class ResetDevicesHub : Hub
    {
        public async Task ResetDevice()
        {
            await Clients.All.SendAsync("ResetReceived");
        }
    }
}
