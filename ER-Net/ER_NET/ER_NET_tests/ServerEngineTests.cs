using System;
using System.Net;
using System.Runtime.InteropServices;
using System.Threading;
using System.Threading.Tasks;
using ER_NET.Server;
using ER_NET.Shared;
using ER_NET_tests.Mocks;
using Xunit;

namespace ER_NET_tests
{
    public class ServerEngineTests
    {
        [Fact]
        void DeviceRegisteredTest()
        {
            var parser = new MockCommunicationParser();
            var server = new MockDiscoveryServer();
            var tcpSender = new MockCommunicationSender();
            var engine = new ErNetServerEngine(parser, server, tcpSender);

            string name = "testPuzzle";
            var testJson = $"ER-NET\n{{\"Name\":\"{name}\",\"MessageType\":\"DiscoveryAcknowledge\"}}";
            var eventArgs = new CommunicationEventArgs(Message.FromJson(testJson), IPAddress.Parse("192.168.2.10"));
            parser.RaiseTcpEvent(eventArgs);

            Thread.Sleep(TimeSpan.FromSeconds(5));

            Assert.True(engine.IsDeviceConnected(name));
        }

        [Fact]
        void DeviceTimeOutTest()
        {
            var parser = new MockCommunicationParser();
            var server = new MockDiscoveryServer();
            var tcpSender = new MockCommunicationSender();
            var engine = new ErNetServerEngine(parser, server, tcpSender);

            string name = "testPuzzle";
            var testJson = $"ER-NET\n{{\"Name\":\"{name}\",\"MessageType\":\"DiscoveryAcknowledge\"}}";
            var eventArgs = new CommunicationEventArgs(Message.FromJson(testJson), IPAddress.Parse("192.168.2.10"));
            parser.RaiseTcpEvent(eventArgs);

            Thread.Sleep(TimeSpan.FromSeconds(12));

            Assert.False(engine.IsDeviceConnected(name));
        }

        [Fact]
        void NullMessageTest()
        {
            var parser = new MockCommunicationParser();
            var server = new MockDiscoveryServer();
            var tcpSender = new MockCommunicationSender();
            var engine = new ErNetServerEngine(parser, server, tcpSender);

            string name = "testPuzzle";
            var eventArgs = new CommunicationEventArgs(null, IPAddress.Parse("192.168.2.10"));
            parser.RaiseTcpEvent(eventArgs);
            Thread.Sleep(TimeSpan.FromSeconds(5));
            Assert.False(engine.IsDeviceConnected(name));
        }

        [Fact]
        void UpdateTimeTest()
        {
            var parser = new MockCommunicationParser();
            var server = new MockDiscoveryServer();
            var tcpSender = new MockCommunicationSender();
            var engine = new ErNetServerEngine(parser, server, tcpSender);

            string name = "testPuzzle";
            var testJson = $"ER-NET\n{{\"Name\":\"{name}\",\"MessageType\":\"DiscoveryAcknowledge\"}}";
            var eventArgs = new CommunicationEventArgs(Message.FromJson(testJson), IPAddress.Parse("192.168.2.10"));

            //Run 3 times to see if the time gets updated
            for (int i = 0; i < 3; i++)
            {
                parser.RaiseTcpEvent(eventArgs);
                Thread.Sleep(TimeSpan.FromSeconds(3));
            }

            Assert.True(engine.IsDeviceConnected(name));
        }

        [Fact]
        void DiscoveryAcknowledgeTest()
        {
            var parser = new MockCommunicationParser();
            var server = new MockDiscoveryServer();
            var tcpSender = new MockCommunicationSender();
            var engine = new ErNetServerEngine(parser, server, tcpSender);

            string name = "testPuzzle";
            var testJson = $"ER-NET\n{{\"Name\":\"{name}\",\"MessageType\":\"DiscoveryAcknowledge\"}}";
            var eventArgs = new CommunicationEventArgs(Message.FromJson(testJson), IPAddress.Parse("192.168.2.10"));
            parser.RaiseTcpEvent(eventArgs);

            Assert.True(server.IsDiscoveryAck);
        }

        [Fact]
        void CalculateSolutionTest()
        {
            var parser = new MockCommunicationParser();
            var server = new MockDiscoveryServer();
            var tcpSender = new MockCommunicationSender();
            var engine = new ErNetServerEngine(parser, server, tcpSender);

            var solution = engine.CalculateSolution(9, 9, 9, 9);
            Assert.True(solution == 96822);
            solution = engine.CalculateSolution(1, 1, 1, 1);
            Assert.True(solution == 10750);
        }

        [Fact]
        void GetDisplayNumberByNameTest()
        {
            var parser = new MockCommunicationParser();
            var server = new MockDiscoveryServer();
            var tcpSender = new MockCommunicationSender();
            var engine = new ErNetServerEngine(parser, server, tcpSender);

            var solution = engine.GenerateNewSolution();
            var analogPuzzle = engine.GetDisplayNumberByName("AnalogPuzzle");
            var digigtalPuzzle = engine.GetDisplayNumberByName("DigitalPuzzle");
            var softwarePuzzle = engine.GetDisplayNumberByName("SoftwarePuzzle");
            var fecPuzzle = engine.GetDisplayNumberByName("FecPuzzle");

            var calculatedSolution = engine.CalculateSolution(analogPuzzle, digigtalPuzzle, softwarePuzzle, fecPuzzle);

            Assert.True(solution == calculatedSolution);
        }

        [Fact]
        void GetDisplayNumberInvalidInputTest()
        {
            var parser = new MockCommunicationParser();
            var server = new MockDiscoveryServer();
            var tcpSender = new MockCommunicationSender();
            var engine = new ErNetServerEngine(parser, server, tcpSender);

            Assert.True(engine.GetDisplayNumberByName("test") == -1);
        }
    }
}