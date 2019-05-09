using System;
using System.Net;
using System.Text;
using Newtonsoft.Json;

namespace ER_NET.Shared
{
    public class Message
    {
        //[JsonProperty("Guid")]
        //public Guid Id { get; set; }
        public string Name { get; set; }
        public string MessageType { get; set; }
        public string Value { get; set; }

        /// <summary>
        /// Returns the Message extracted from the json string.
        /// </summary>
        /// <param name="json">The received Discovery json string</param>
        /// <returns>The Message when a valid message is passed as JSON, otherwise returns null.</returns>
        public static Message FromJson(string json)
        {
            if (json.StartsWith("ER-NET\n"))
            {
                //Indicating this is an ER-NET message
                json = json.Substring("ER-NET\n".Length);
                var message = JsonConvert.DeserializeObject<Message>(json);
                return message;
            }

            return null;
        }

        public string ToJsonString()
        {
            return $"ER-NET\n{JsonConvert.SerializeObject(this)}";
        }

        public byte[] ToBytes()
        {
            return Encoding.ASCII.GetBytes(ToJsonString());
        }

        public override string ToString()
        {
            return ToJsonString();
        }
    }
}