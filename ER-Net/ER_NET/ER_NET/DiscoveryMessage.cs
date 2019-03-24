using System;
using Newtonsoft.Json;
using Newtonsoft.Json.Serialization;

namespace PuzzleClient
{
    public class DiscoveryMessage
    {
        [JsonProperty("Guid")]
        public Guid Id { get; set; }
        public string MessageType { get; set; }
        
        /// <summary>
        /// Returns the DiscoveryMessage extracted from the json string.
        /// </summary>
        /// <param name="json">The received Discovery json string</param>
        /// <returns>The DiscoveryMessage when a valid message is passed as JSON, otherwise returns null.</returns>
        public static DiscoveryMessage FromJson(string json)
        {
            if (json.StartsWith("ER-NET\n"))
            {
                //Indicating this is an ER-NET message
                json = json.Substring("ER-NET\n".Length);
                var message = JsonConvert.DeserializeObject<DiscoveryMessage>(json);
                return message;
            }

            return null;
        }

        public string ToJsonString()
        {
            return $"ER-NET\n{JsonConvert.SerializeObject(this)}";
        }
    }
}