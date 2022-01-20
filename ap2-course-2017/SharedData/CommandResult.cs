using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SharedData
{
    /// <summary>
    /// define the possible commands.
    /// </summary>
    public enum Command
    {
        Undefined = -1,
        Generate,
        Solve,
        Start,
        List,
        Join,
        Play,
        Close
    }

    /// <summary>
    /// a command result,that contains the info
    /// that the client needs from the command.
    /// </summary>
    public class CommandResult
    {
        public bool Success { get; private set; }
        public Command Command { get; private set; }
        public string Data { get; private set; }
        public bool KeepConnection { get; private set; }

        /// <summary>
        /// Initializes a new instance of the <see cref="CommandResult"/> class.
        /// </summary>
        /// <param name="success">if set to <c>true</c> [success].</param>
        /// <param name="cmd">The command.</param>
        /// <param name="payload">The payload.</param>
        /// <param name="keepConnection">if set to <c>true</c> [keep connection].</param>
        public CommandResult(bool success, Command cmd, string payload, bool keepConnection)
        {
            Success = success;
            Command = cmd;
            Data = payload;
            KeepConnection = keepConnection;
        }

        /// <summary>
        /// Prevents a default instance of the 
        /// <see cref="CommandResult"/> class from being created.
        /// </summary>
        private CommandResult() { }

        /// <summary>
        /// To the json.
        /// </summary>
        /// <returns></returns>
        public string ToJSON()
        {
            JObject j = new JObject();
            j["Success"] = Success;
            j["Data"] = Data;
            j["KeepConnection"] = KeepConnection;
            j["Command"] = Command.ToString();
            return j.ToString();
        }

        /// <summary>
        /// Froms the json.
        /// </summary>
        /// <param name="str">The string.</param>
        /// <returns></returns>
        public static CommandResult FromJSON(string str)
        {
            CommandResult cr = new CommandResult();
            JObject j = JObject.Parse(str);
            cr.Success = (bool)j["Success"];
            cr.Data = (string)j["Data"];

            string s = (string)j["Command"];
            Command c = (Command)Enum.Parse(typeof(Command), s);
            cr.Command = c;

            cr.KeepConnection = (bool)j["KeepConnection"];
            return cr;
        }
    }
}
