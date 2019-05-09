using System;
using System.Text;
using System.Threading;
using ER_NET.Server;
using ER_NET.Shared;
using ER_NET_tests.Mocks;
using Xunit;

namespace ER_NET_tests
{
    public class DiscoveryTests
    {
        [Fact]
        public void DoDiscoveryTest()
        {
            var sender = new MockCommunicationSender();
            var discoveryServer = new DiscoveryServer(sender);

            discoveryServer.DoDiscovery();

            string name = "ControlUnit";
            var json = $"ER-NET\n{{\"Name\":\"{name}\",\"MessageType\":\"Discovery\"}}";
            var message = Message.FromJson(json);

            var lastMessage = Message.FromJson(Encoding.ASCII.GetString(sender.lastMessageSent));

            Assert.True(lastMessage.Name == name);
            Assert.True(lastMessage.MessageType == "Discovery");
        }

        [Fact]
        public void DoContinousDiscoveryTest()
        {
            var sender = new MockCommunicationSender();
            var discoveryServer = new DiscoveryServer(sender);

            discoveryServer.ContinuousDiscovery();

            string name = "ControlUnit";
            var json = $"ER-NET\n{{\"Name\":\"{name}\",\"MessageType\":\"Discovery\"}}";
            var message = Message.FromJson(json);

            Thread.Sleep(TimeSpan.FromSeconds(3));

            var lastMessage = Message.FromJson(Encoding.ASCII.GetString(sender.lastMessageSent));

            Assert.True(lastMessage.Name == name);
            Assert.True(lastMessage.MessageType == "Discovery");

            sender.lastMessageSent = null;
            
            Thread.Sleep(TimeSpan.FromSeconds(3));

            
            lastMessage = Message.FromJson(Encoding.ASCII.GetString(sender.lastMessageSent));

            Assert.True(lastMessage.Name == name);
            Assert.True(lastMessage.MessageType == "Discovery");
        }
    }
}