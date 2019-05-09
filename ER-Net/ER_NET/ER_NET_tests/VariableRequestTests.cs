using System;
using System.Net;
using System.Text;
using ER_NET.Server;
using ER_NET.Shared;
using ER_NET_tests.Mocks;
using Xunit;

namespace ER_NET_tests
{
    public class VariableRequestTests
    {
        [Fact]
        void GetNumberTest()
        {
            var parser = new MockCommunicationParser();
            var server = new MockDiscoveryServer();
            var tcpSender = new MockCommunicationSender();
            var engine = new ErNetServerEngine(parser, server, tcpSender);
            engine.GenerateNewSolution();

            string name = "DigitalPuzzle";
            var testJson = $"ER-NET\n{{\"Name\":\"{name}\",\"MessageType\":\"DiscoveryAcknowledge\"}}";
            var eventArgs = new CommunicationEventArgs(Message.FromJson(testJson), IPAddress.Parse("192.168.2.10"));
            parser.RaiseTcpEvent(eventArgs);
            
            testJson = $"ER-NET\n{{\"Name\":\"{name}\",\"MessageType\":\"GetDisplayNumber\",\"Value\":\"null\"}}";
            eventArgs = new CommunicationEventArgs(Message.FromJson(testJson), IPAddress.Parse("192.168.2.10"));
            parser.RaiseTcpEvent(eventArgs);

            Assert.True(tcpSender.lastIp.Equals(IPAddress.Parse("192.168.2.10")));
            var jsonResponse = Encoding.ASCII.GetString(tcpSender.lastMessageSent);
            var message = Message.FromJson(jsonResponse);
            Assert.True(message.MessageType == "DisplayNumber");
            int DisplayNumber;
            Assert.True(int.TryParse(message.Value, out DisplayNumber));
            Assert.InRange(DisplayNumber, 1, 9);
        }
    }
}