using System;
using System.Collections.Generic;
using System.Net;
using System.Threading.Tasks;
using ER_NET.Shared;

using Timer = System.Timers.Timer;

namespace ER_NET.Server
{
    public class ErNetServerEngine
    {
        public static ErNetServerEngine Instance
        {
            get
            {
                if (_instance == null)
                {
                    var parser = new UdpParser();
                    var server = new DiscoveryServer(UdpSender.Instance);
                    var udpSender = UdpSender.Instance;
                    _instance = new ErNetServerEngine(parser, server, udpSender);
                }

                return _instance;
            }
        }

        public List<string> Devices
        {
            get
            {
                var devices = new List<string>();
                foreach (var key in _connectedDevices.Keys)
                {
                    var displayNumber = GetDisplayNumberByName(key);
                    if (displayNumber == -1)
                    {
                        devices.Add(key + "\t ");
                    }
                    else
                    {
                        devices.Add(key + "\t Nummer: " + displayNumber);
                    }
                }

                return devices;
            }
        }

        public int Solution => _solution;

        #region Events

        public event EventHandler<EventArgs> OnDevicesListChanged;
        public event EventHandler<EventArgs> OnReset;
        public event EventHandler<EventArgs> OnTimerTick;
        public event EventHandler<EventArgs> OnStatusChanged;
        public event EventHandler<EventArgs> OnRoomSolved;

        #endregion

        private static ErNetServerEngine _instance = null;

        private Dictionary<String, Device> _connectedDevices = new Dictionary<String, Device>();
        private ICommunicationParser _parser;
        private IDiscoveryServer _discoveryServer;
        private ICommunicationSender _communicationSender;

        private const int NumOfPuzzles = 4;
        private List<string> _puzzleNames;

        private Random _randomGenerator = new Random();
        private int _solution;
        private List<int> _digits = new List<int>();

        private System.Timers.Timer _timer;
        private TimeSpan _timeLeft;
        public string TimeLeft => _timeLeft.ToString(@"mm\:ss");
        public string Status;

        public ErNetServerEngine(ICommunicationParser parser, IDiscoveryServer discoveryServer,
            ICommunicationSender communicationSender)
        {
            _parser = parser;
            _discoveryServer = discoveryServer;
            _communicationSender = communicationSender;
            var guid = Guid.NewGuid();
            _discoveryServer.Name = "ControlUnit";
            _discoveryServer.ContinuousDiscovery();

            _timer = new Timer(1000);
            _timer.AutoReset = true;
            _timer.Elapsed += (sender, args) =>
            {
                _timeLeft = _timeLeft.Subtract(TimeSpan.FromSeconds(1));
                RaiseTimerTickEvent();
                if ((int)_timeLeft.TotalMilliseconds == 0)
                {
                    _timer.Stop();
                    Status = "Failed";
                    RaiseStatusChangedEvent();
                }
            };
            _timer.Enabled = false;
            _timeLeft = TimeSpan.FromMinutes(10);

            Task.Run(async () =>
            {
                while (true)
                {
                    CheckConnectedDevices();
                    await Task.Delay(3);
                }
            });

            GenerateNewSolution();
            _parser.Start();
            _parser.OnCommunicationEvent += OnParserOnOnCommunicationEvent;

            Status = "Idle";
        }

        #region RaiseEvents

        protected virtual void RaiseDeviceListChangedEvent()
        {
            OnDevicesListChanged?.Invoke(this, new EventArgs());
        }

        protected virtual void RaiseResetEvent()
        {
            OnReset?.Invoke(this, new EventArgs());
        }

        protected virtual void RaiseTimerTickEvent()
        {
            OnTimerTick?.Invoke(this, new EventArgs());
        }

        protected virtual void RaiseStatusChangedEvent()
        {
            OnStatusChanged?.Invoke(this, new EventArgs());
        }

        protected virtual void RaiseRoomSolvedEvent()
        {
            OnRoomSolved?.Invoke(this, new EventArgs());
        }

        #endregion

        public void StartTimer()
        {
            _timer.Start();
            Status = "Started";
            RaiseStatusChangedEvent();
        }

        public void StopTimer()
        {
            _timer.Stop();
            Status = "Stopped";
            RaiseStatusChangedEvent();
        }
        public bool SetPuzzles(List<string> puzzles)
        {
            if (puzzles.Count == NumOfPuzzles)
            {
                _puzzleNames = puzzles;
            }

            return false;
        }

        /// <summary>
        /// Generate a new solution for the Escape Room
        /// </summary>
        /// <returns>The solution</returns>
        public int GenerateNewSolution()
        {
            int numbersForPuzzles = 0;
            for (int i = 0; i < NumOfPuzzles; i++)
            {
                var number = _randomGenerator.Next(1, 10);
                numbersForPuzzles = (numbersForPuzzles * 10) + number;
            }

            var digitsForPuzzles = GetDigits(numbersForPuzzles);
            _digits = digitsForPuzzles;

            //set the numbers for the correct puzzles
            var analogDigit = digitsForPuzzles[0];
            var digitalDigit = digitsForPuzzles[1];
            var softwareDigit = digitsForPuzzles[2];
            var fecDigit = digitsForPuzzles[3];

            //Calculate the final solution
            var finalSolution = CalculateSolution(analogDigit, digitalDigit, softwareDigit, fecDigit);
            _solution = finalSolution;
            return finalSolution;
        }

        public int GetDisplayNumberByName(string name)
        {
            switch (name)
            {
                case "AnalogPuzzle":
                    return _digits[0];
                case "DigitalPuzzle":
                    return _digits[1];
                case "SoftwarePuzzle":
                    return _digits[2];
                case "FecPuzzle":
                    return _digits[3];
                default:
                    return -1;
            }
        }

        public int CalculateSolution(int analogDigit, int digitalDigit, int softwareDigit, int fecDigit)
        {
            return analogDigit * digitalDigit + ((3500 * softwareDigit + 18000 * fecDigit) / 2) - digitalDigit;
        }

        private async void OnParserOnOnCommunicationEvent(object sender, CommunicationEventArgs eventArgs)
        {
            var addressList = Dns.GetHostEntry(Dns.GetHostName()).AddressList;
            foreach (var ipAddress in addressList)
            {
                if (eventArgs.RemoteIp.Equals(ipAddress))
                {
                    return;
                }
            }

            var message = eventArgs.Message;
            if (message == null)
            {
                return;
            }

            //Console.WriteLine($"Received MessageType {message.MessageType} from GUID {message.Id} with IP {eventArgs.RemoteIp}");
            if (message.MessageType == "DiscoveryAcknowledge")
            {
                if (!_connectedDevices.ContainsKey(message.Name))
                {
                    _connectedDevices.Add(message.Name, new Device(message.Name, eventArgs.RemoteIp));
                    //Device added, so raise event
                    RaiseDeviceListChangedEvent();
                }
                else
                {
                    _connectedDevices[message.Name].LastConnected = DateTime.UtcNow;
                }

                _discoveryServer.DiscoveryAcknowledgeReceived(message, eventArgs.RemoteIp);
            }
            else if (message.MessageType == "GetDisplayNumber")
            {
                if (_connectedDevices.ContainsKey(message.Name))
                {
                    var displayNumber = GetDisplayNumberByName(message.Name);

                    var response = new Message
                    { Name = _discoveryServer.Name, MessageType = "DisplayNumber", Value = displayNumber.ToString() };
                    await _communicationSender.SendMessageAsync(response.ToBytes(), eventArgs.RemoteIp,
                        CommunicationPorts.CommunicationPort);
                }
            }
            else if (message.MessageType == "GetSolution")
            {
                var response = new Message
                {
                    Name = _discoveryServer.Name,
                    MessageType = "Solution",
                    Value = _solution.ToString()
                };
                await _communicationSender.SendMessageAsync(response.ToBytes(), eventArgs.RemoteIp,
                    CommunicationPorts.CommunicationPort);
            }
            else if (message.MessageType == "RoomSolved")
            {
                var response = new Message
                {
                    Name = _discoveryServer.Name,
                    MessageType = "RoomSolved",
                    Value = null
                };
                await _communicationSender.SendMessageAsync(response.ToBytes(), eventArgs.RemoteIp,
                    CommunicationPorts.CommunicationPort);
                RaiseRoomSolvedEvent();
                _timer.Stop();
                Status = "Solved";
                RaiseStatusChangedEvent();
            }
        }

        public async void ResetDevices()
        {
            GenerateNewSolution();
            foreach (var device in _connectedDevices.Values)
            {
                device.Reset();
                var message = new Message
                {
                    Name = _discoveryServer.Name,
                    MessageType = "Reset"
                };
                await _communicationSender.SendMessageAsync(message.ToBytes(), device.IpAddress,
                    CommunicationPorts.CommunicationPort);
            }

            _timer.Stop();
            _timeLeft = TimeSpan.FromMinutes(10);

            Status = "Idle";
            RaiseTimerTickEvent();
            RaiseResetEvent();
            RaiseStatusChangedEvent();
        }

        public bool IsDeviceConnected(string name)
        {
            return _connectedDevices.ContainsKey(name);
        }

        private void CheckConnectedDevices()
        {
            var now = DateTime.UtcNow;
            var removeList = new List<string>();
            foreach (var device in _connectedDevices)
            {
                var time = device.Value.LastConnected;
                var difference = now - time;
                if (difference.Seconds > 10)
                {
                    removeList.Add(device.Key);
                    //We don't remove the device from the dictionary here because that will modify the collection and trow an exception
                }

                //Console.WriteLine("Device ID: {0} with IP {1} last seen at {2}", device.Value.Id, device.Value.IpAddress, device.Value.LastConnected);
            }

            //After enumerating over the whole list, remove the devices
            foreach (var name in removeList)
            {
                _connectedDevices.Remove(name);
            }

            //One or more items where removed so we raise the vent
            if (removeList.Count >= 1)
            {
                RaiseDeviceListChangedEvent();
            }
        }

        private static List<int> GetDigits(int source)
        {
            var digits = new List<int>();
            int individualFactor = 0;
            int tennerFactor = Convert.ToInt32(Math.Pow(10, source.ToString().Length));
            do
            {
                source -= tennerFactor * individualFactor;
                tennerFactor /= 10;
                individualFactor = source / tennerFactor;

                digits.Add(individualFactor);
            } while (tennerFactor > 1);

            return digits;
        }
    }
}