using System;
using System.Collections.Generic;

namespace ControlUnit_webserver.Models
{
    public class ControlPageViewModel
    {
        public List<string> Devices { get; set; }
        public int Solution { get; set; }
        public string PlayTime { get; set; }
        public string Status { get; set; }
    }
}