using System.Net;
using ER_NET.Shared;
using FluentAssertions;
using Xunit;

namespace ER_NET_tests
{
    public class TcpParserTests
    {
        [Fact]
        public void Correct_TCP_received()
        {
            string mockMessage =
                "ER-NET\n{\"Guid\":\"e6ad6f00-82f2-40eb-855a-54133ba29b70\",\"MessageType\":\"DiscoveryAcknowledge\"}";
            var parser = new TcpParser();
            Assert.Raises<CommunicationEventArgs>(handler => parser.OnCommunicationEvent += handler,
                handler => parser.OnCommunicationEvent -= handler, () => { parser.OnMessageReceived(mockMessage, IPAddress.Parse("192.168.2.10")); });
        }

        [Fact]
        public void Wrong_TCP_received()
        {
            string mockMessage =
                "\n{\"Guid\":\"e6ad6f00-82f2-40eb-855a-54133ba29b70\",\"MessageType\":\"DiscoveryAcknowledge\"}";
            var parser = new TcpParser();
            using (var monitor = parser.Monitor())
            {
                parser.OnMessageReceived(mockMessage, IPAddress.Parse("192.168.2.10"));
                monitor.Should().NotRaise("OnCommunicationEvent");
            }
        }
    }
}