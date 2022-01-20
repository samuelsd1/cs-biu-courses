using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SharedData
{
    /// <summary>
    /// an enum that define the type
    /// of messages a client can get.
    /// </summary>
    public enum MessageType
    {
        CommandResult,
        Notification
    }

    /// <summary>
    /// a class that contains a massage to the client
    /// </summary>
    public class Message
    {
        public MessageType MessageType { get; private set; }
        public string Data { get; private set; }

        /// <summary>
        /// Initializes a new instance of the <see cref="Message"/> class.
        /// </summary>
        /// <param name="type">The type.</param>
        /// <param name="data">The data.</param>
        public Message(MessageType type, string data)
        {
            this.MessageType = type;
            this.Data = data;
        }

        /// <summary>
        /// Prevents a default instance of the
        ///  <see cref="Message"/> class from being created.
        /// </summary>
        private Message() { }

        /// <summary>
        /// To the json.
        /// </summary>
        /// <returns></returns>
        public string ToJSON()
        {
            JObject j = new JObject();

            j["MessageType"] = MessageType.ToString();
            j["Data"] = Data;
            return j.ToString();
        }

        /// <summary>
        /// Froms the json.
        /// </summary>
        /// <param name="s">The s.</param>
        /// <returns></returns>
        public static Message FromJSON(string s)
        {
            Message m = new Message();
            JObject j = JObject.Parse(s);

            string str = (string)j["MessageType"];
            MessageType mt = (MessageType)Enum.Parse(typeof(MessageType), str);
            m.MessageType = mt;

            m.Data = (string)j["Data"];
            return m;


        }
    }

}
