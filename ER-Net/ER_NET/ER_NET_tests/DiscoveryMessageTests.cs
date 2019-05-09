using System;
using ER_NET.Shared;
using Microsoft.VisualStudio.TestPlatform.ObjectModel;
using Xunit;

namespace PuzzleClientTests
{
    public class DiscoveryMessageTests
    {
        [Fact]
        public void FromJsonCorrect()
        {
            string name = "testPuzzle";
            var json = $"ER-NET\n{{\"Name\":\"{name}\",\"MessageType\":\"Discovery\"}}";
            var message = Message.FromJson(json);
            Assert.True(message.MessageType == "Discovery");
            Assert.True(message.Name == name);
        }

        [Fact]
        public void FromJsonIncorrect()
        {
            var json = "{\"Guid\":\"e6ad6f00-82f2-40eb-855a-54133ba29b70\",\"MessageType\":\"Discovery\"}";
            var message = Message.FromJson(json);
            Assert.Null(message);
        }

        [Fact]
        public void ToJson()
        {
            var message = new Message
            {
                Name = "testPuzzle",
                MessageType = "DiscoveryAcknowledge"
            };
            var json = message.ToJsonString();
            Assert.True(json == $"ER-NET\n{{\"Name\":\"{message.Name}\",\"MessageType\":\"DiscoveryAcknowledge\",\"Value\":null}}");
        }
    }
}