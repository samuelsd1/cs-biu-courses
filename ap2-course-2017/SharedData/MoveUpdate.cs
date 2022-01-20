using MazeLib;
using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SharedData
{

    /// <summary>
    /// a class that contains a move update
    /// of the other player, to the client
    /// </summary>
    public class MoveUpdate
    {
        public string GameName { get; private set; }
        public Direction Direction { get; private set; }

        /// <summary>
        /// Initializes a new instance of the <see cref="MoveUpdate"/> class.
        /// </summary>
        /// <param name="gameName">Name of the game.</param>
        /// <param name="d">The d.</param>
        public MoveUpdate(string gameName, Direction d)
        {
            this.GameName = gameName;
            this.Direction = d;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MoveUpdate"/> class.
        /// </summary>
        private MoveUpdate()
        {
        }

        /// <summary>
        /// To the json.
        /// </summary>
        /// <returns></returns>
        public string ToJSON()
        {
            JObject j = new JObject();

            j["GameName"] = GameName;
            j["Direction"] = Direction.ToString();
            return j.ToString();
        }

        /// <summary>
        /// Froms the json.
        /// </summary>
        /// <param name="s">The s.</param>
        /// <returns></returns>
        public static MoveUpdate FromJSON(string s)
        {
            JObject j = JObject.Parse(s);
            MoveUpdate mu = new MoveUpdate();
            mu.GameName = (string)j["GameName"];
            string str = (string)j["Direction"];
            Direction d = (Direction)Enum.Parse(typeof(Direction), str);
            mu.Direction = d;
            return mu;
        }
    }
}
