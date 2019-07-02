using System;
using System.Collections.Generic;
using System.Text;
using ER_NET.Client;
using ER_NET_tests.Mocks;
using Xunit;

namespace ER_NET_tests
{
    public class ClientEngineTests
    {
        [Fact]
        void InstanceTest()
        {
            Assert.Null(ErNetClientEngine.Instance);

            var engine = new ErNetClientEngine("testPuzzle", new MockCommunicationSender(), new MockCommunicationParser());

            Assert.NotNull(ErNetClientEngine.Instance);
            Assert.True(ErNetClientEngine.Instance == engine);
        }

        [Fact]
        async void NoIpSetTest()
        {
            var engine = new ErNetClientEngine("testPuzzle", new MockCommunicationSender(), new MockCommunicationParser());

            var number = await engine.GetDisplayNumberAsync();

            Assert.Equal(-1, number);
        }
    }
}
