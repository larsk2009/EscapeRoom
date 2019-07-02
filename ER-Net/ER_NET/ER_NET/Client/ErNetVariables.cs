using System;
using System.Collections.Generic;
using System.Net;
using System.Threading;
using System.Threading.Tasks;
using ER_NET.Server;
using ER_NET.Shared;

namespace ER_NET.Client
{
    public class ErNetVariables
    {
        private ICommunicationSender _sender;

        private Queue<Message> _receivedMessages = new Queue<Message>();

        public ErNetVariables(ICommunicationSender sender)
        {
            _sender = sender;
        }

        /// <summary>
        /// Handle a received message
        /// </summary>
        /// <param name="e">CommunicationEventArgs that contains the message and the IP of the control unit</param>
        /// <returns>True when this function handled the event, false when this function didn't handle it</returns>
        public bool OnCommunication(CommunicationEventArgs e)
        {
            var message = e.Message;

            if (message.MessageType == "DisplayNumber")
            {
                _receivedMessages.Enqueue(message);
            }
            else if (message.MessageType == "Solution")
            {
                _receivedMessages.Enqueue(message);
            }
            else
            {
                return false;
            }

            return true;
        }

        public async Task<int> GetDisplayNumberAsync(string puzzleName, IPAddress controlunitIp)
        {
            if (Equals(controlunitIp, IPAddress.None))
            {
                //No IP has been set yet so we return -1
                //return -1;
            }

            var message = new Message
            {
                Name = puzzleName,
                MessageType = "GetDisplayNumber"
            };

            await _sender.SendMessageAsync(message.ToBytes(), controlunitIp, CommunicationPorts.CommunicationPort);

            if (await CheckQueueAsync("DisplayNumber"))
            {
                var receivedMessage = _receivedMessages.Dequeue();
                int displayNumber;
                if (Int32.TryParse(receivedMessage.Value, out displayNumber))
                {
                    return displayNumber;
                }
            }
            return -1;
        }

        public async Task<int> GetSolutionAsync(string puzzleName, IPAddress controlunitIP)
        {
            var message = new Message
            {
                Name = puzzleName,
                MessageType = "GetSolution"
            };

            await _sender.SendMessageAsync(message.ToBytes(), controlunitIP, CommunicationPorts.CommunicationPort);

            if (await CheckQueueAsync("Solution"))
            {
                var receivedMessage = _receivedMessages.Dequeue();
                int solution;
                if (Int32.TryParse(receivedMessage.Value, out solution))
                {
                    return solution;
                }
            }

            return -1;
        }

        private async Task<bool> CheckQueueAsync(string messageType)
        {
            const int timeout = 3000;
            var tokenSource = new CancellationTokenSource();
            tokenSource.CancelAfter(timeout);
            try
            {
                return await Task.Run(() =>
                {
                    while (true)
                    {
                        Message message;
                        if (_receivedMessages.TryPeek(out message))
                        {
                            if (message != null && message.MessageType == messageType)
                            {
                                return true;
                            }
                            else
                            {
                                Console.WriteLine("error");
                            }
                        }
                    }
                }, tokenSource.Token);
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
            }

            return false;
        }
    }
}