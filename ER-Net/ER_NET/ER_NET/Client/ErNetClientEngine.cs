using System;
using System.Net;
using System.Threading.Tasks;
using ER_NET.Client;
using ER_NET.Server;
using ER_NET.Shared;

namespace ER_NET.Client
{
    public class ErNetClientEngine
    {
        private ICommunicationSender _sender;
        private ICommunicationParser _parser;

        private string _puzzleName;
        private IPAddress _controlunitIP;
        
        public event EventHandler OnReset;

        public ErNetVariables Variables;

        /// <summary>
        /// Preferably use the public Instance for the client. The public constructor is used in tests
        /// </summary>
        public ErNetClientEngine(string PuzzleName, ICommunicationSender sender, ICommunicationParser parser)
        {
            _puzzleName = PuzzleName;
            _parser = parser;
            _sender = sender;
            parser.OnCommunicationEvent += OnCommunication;
            parser.Start();
            
            Variables = new ErNetVariables(_sender);
        }

        #region GetVariables
        
        public async Task<int> GetDisplayNumber() => await Variables.GetDisplayNumber(_puzzleName, _controlunitIP);
        public async Task<int> GetSolution() => await Variables.GetSolution(_puzzleName, _controlunitIP);
        
        #endregion

        private void OnCommunication(object sender, CommunicationEventArgs e)
        {
            var message = e.Message;
            if (message.MessageType == "Discovery")
            {
                //Send DiscoveryAcknowledge
                OnDiscovery(message, e.RemoteIp);
            } else if (message.MessageType == "Reset")
            {
                RaiseOnReset();
            }
            else
            {
                Variables.OnCommunication(e);
            }
        }

        private async void OnDiscovery(Message message, IPAddress remoteIp)
        {
            var responseMessage = new Message
            {
                Name = _puzzleName,
                MessageType = "DiscoveryAcknowledge"
            };
            _controlunitIP = remoteIp;

            await _sender.SendMessageAsync(responseMessage.ToBytes(), remoteIp, CommunicationPorts.CommunicationPort);
        }

        #region Events

        protected virtual void RaiseOnReset()
        {
            OnReset?.Invoke(this, new EventArgs());
        }

        #endregion
    }
}