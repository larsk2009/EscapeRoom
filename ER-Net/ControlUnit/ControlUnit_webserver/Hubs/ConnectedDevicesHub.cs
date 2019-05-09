using System.Collections.Generic;
using System.Threading.Tasks;
using ER_NET.Server;
using Microsoft.AspNetCore.SignalR;

namespace ControlUnit_webserver.Hubs
{
    public class ConnectedDevicesHub : Hub
    {
        public async Task AddDevice(List<string> devices)
        {
            await Clients.All.SendAsync("AddDevice", devices);
        }

        public async Task Reset()
        {
            ErNetServerEngine.Instance.ResetDevices();
        }
    }
}