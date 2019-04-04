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
            var parser = new MockTcpParser();
            var engine = new ErNetServerEngine(parser);

            Guid guid = Guid.Parse("e57d5d61-4e98-42b3-a096-81fd249b2cef");
            var testJson = $"ER-NET\n{{\"Guid\":\"{guid}\",\"MessageType\":\"DiscoveryAcknowledge\"}}";
            var eventArgs = new TcpEventArgs(Message.FromJson(testJson), IPAddress.Parse("192.168.2.10"));
            parser.RaiseTcpEvent(eventArgs);

            Thread.Sleep(TimeSpan.FromSeconds(5));
            
            Assert.True(engine.IsDeviceConnected(guid));
        }
        
        [Fact]
        void DeviceTimeOutTest()
        {
            var parser = new MockTcpParser();
            var engine = new ErNetServerEngine(parser);

            Guid guid = Guid.Parse("e57d5d61-4e98-42b3-a096-81fd249b2cef");
            var testJson = $"ER-NET\n{{\"Guid\":\"{guid}\",\"MessageType\":\"DiscoveryAcknowledge\"}}";
            var eventArgs = new TcpEventArgs(Message.FromJson(testJson), IPAddress.Parse("192.168.2.10"));
            parser.RaiseTcpEvent(eventArgs);

            Thread.Sleep(TimeSpan.FromSeconds(12));

            Assert.False(engine.IsDeviceConnected(guid));
        }

        [Fact]
        void NullMessageTest()
        {
            var parser = new MockTcpParser();
            var engine = new ErNetServerEngine(parser);

            var guid = Guid.NewGuid();
            var eventArgs = new TcpEventArgs(null, IPAddress.Parse("192.168.2.10"));
            parser.RaiseTcpEvent(eventArgs);
            Thread.Sleep(TimeSpan.FromSeconds(5));
            Assert.False(engine.IsDeviceConnected(guid));
        }

        [Fact]
        void UpdateTimeTest()
        {
            var parser = new MockTcpParser();
            var engine = new ErNetServerEngine(parser);

            Guid guid = Guid.Parse("e57d5d61-4e98-42b3-a096-81fd249b2cef");
            var testJson = $"ER-NET\n{{\"Guid\":\"{guid}\",\"MessageType\":\"DiscoveryAcknowledge\"}}";
            var eventArgs = new TcpEventArgs(Message.FromJson(testJson), IPAddress.Parse("192.168.2.10"));

            //Run 3 times to see if the time gets updated
            for (int i = 0; i < 3; i++)
            {
                parser.RaiseTcpEvent(eventArgs);
                Thread.Sleep(TimeSpan.FromSeconds(3));
            }
            
            Assert.True(engine.IsDeviceConnected(guid));
        }
    }
}