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

        private ErNetVariables Variables;

        private static ErNetClientEngine _instance = null;
        public static ErNetClientEngine Instance => _instance;

        /// <summary>
        /// Preferably use the public Instance for the client. The public constructor is used in tests
        /// </summary>
        public ErNetClientEngine(string puzzleName, ICommunicationSender sender, ICommunicationParser parser)
        {
            _puzzleName = puzzleName;
            _parser = parser;
            _sender = sender;
            _controlunitIP = IPAddress.None;
            parser.OnCommunicationEvent += OnCommunication;
            parser.Start();
            
            Variables = new ErNetVariables(_sender);

            _instance = this;
        }

        #region GetVariables
        
        public async Task<int> GetDisplayNumberAsync() => await Variables.GetDisplayNumberAsync(_puzzleName, _controlunitIP);
        public async Task<int> GetSolutionAsync() => await Variables.GetSolutionAsync(_puzzleName, _controlunitIP);
        
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