using System;
using System.Net;

namespace ER_NET.Shared
{
    public class Device
    {
        //public Guid Id { get; set; }
        public string Name { get; set; }
        public IPAddress IpAddress { get; set; }
        public DeviceType Type { get; set; }
        public DateTime LastConnected { get; set; }
        //public int DisplayNumber { get; set; }

        public Device(String name, IPAddress ipAddress, DeviceType type = DeviceType.Unknown)
        {
            //Id = id;
            Name = name;
            IpAddress = ipAddress;
            Type = type;
            LastConnected = DateTime.UtcNow;
            //DisplayNumber = new Random().Next(1, 10);
        }

        //Placeholder Reset function
        public void Reset()
        {
            //DisplayNumber = new Random().Next(1, 10);
        }
    }
}