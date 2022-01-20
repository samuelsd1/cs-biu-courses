using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SharedData
{

    public class Notification
    {

        /// <summary>
        /// an enum that define the type
        /// of notifications a client can get.
        /// </summary>
        public enum Type
        {
            GameStarted,
            GameOver,
            PlayerMoved
        }

        public Type NotificationType { get; private set; }
        public string Data { get; private set; }

        /// <summary>
        /// Initializes a new instance of the <see cref="Notification"/> class.
        /// </summary>
        /// <param name="type">The type.</param>
        /// <param name="data">The data.</param>
        public Notification(Type type, string data)
        {
            this.NotificationType = type;
            this.Data = data;
        }

        /// <summary>
        /// Prevents a default instance of the 
        /// <see cref="Notification"/> class from being created.
        /// </summary>
        private Notification() { }

        /// <summary>
        /// To the json.
        /// </summary>
        /// <returns></returns>
        public string ToJSON()
        {
            JObject j = new JObject();
            j["Data"] = Data;
            j["NotificationType"] = NotificationType.ToString();
            return j.ToString();
        }

        /// <summary>
        /// Froms the json.
        /// </summary>
        /// <param name="str">The string.</param>
        /// <returns></returns>
        public static Notification FromJSON(string str)
        {
            JObject j = JObject.Parse(str);
            Notification notif = new Notification();
            notif.Data = (string)j["Data"];
            string s = (string)j["NotificationType"];
            Type t = (Type)Enum.Parse(typeof(Type), s);
            notif.NotificationType = t;
            return notif;
        }
    }
}
