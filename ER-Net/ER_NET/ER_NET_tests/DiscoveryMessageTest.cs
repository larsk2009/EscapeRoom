using System;
using ER_NET.Shared;
using Microsoft.VisualStudio.TestPlatform.ObjectModel;
using Xunit;

namespace PuzzleClientTests
{
    public class DiscoveryMessageTest
    {
        [Fact]
        public void FromJsonCorrect()
        {
            var json = "ER-NET\n{\"Guid\":\"e6ad6f00-82f2-40eb-855a-54133ba29b70\",\"MessageType\":\"Discovery\"}";
            var message = DiscoveryMessage.FromJson(json);
            Assert.True(message.MessageType == "Discovery");
            Assert.True(message.Id == Guid.Parse("e6ad6f00-82f2-40eb-855a-54133ba29b70"));
        }

        [Fact]
        public void FromJsonIncorrect()
        {
            var json = "{\"Guid\":\"e6ad6f00-82f2-40eb-855a-54133ba29b70\",\"MessageType\":\"Discovery\"}";
            var message = DiscoveryMessage.FromJson(json);
            Assert.Null(message);
        }

        [Fact]
        public void ToJson()
        {
            var message = new DiscoveryMessage
            {
                Id = Guid.Parse("e6ad6f00-82f2-40eb-855b-54133ba29b75"),
                MessageType = "DiscoveryAcknowledge"
            };
            var json = message.ToJsonString();
            Assert.True(json == "ER-NET\n{\"Guid\":\"e6ad6f00-82f2-40eb-855b-54133ba29b75\",\"MessageType\":\"DiscoveryAcknowledge\"}");
        }
    }
}