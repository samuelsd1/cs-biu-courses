using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

using SharedData;
using Newtonsoft.Json;
using WebApp.Models.Multi;

namespace WebApp.Controllers.Commands
{
    /// <summary>
    /// a class that is in charge of executing the
    /// list command
    /// </summary>
    /// <seealso ServerCommand" />
    class ListCommand : ServerCommand
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="ListCommand" /> class.
        /// </summary>
        /// <param name="model">The model.</param>
        public ListCommand(MultiModel model) : base(model) { }

        /// <summary>
        /// Executes the command.
        /// </summary>
        /// <param name="sender">The sender.</param>
        /// <param name="args">The arguments.</param>
        /// <returns></returns>
        public override CommandResult Execute(IClient sender, string[] args)
        {
            // TODO : check whether we close the connection or leave it open.
            List<string> gameList = _model.GetAvailableGames();
            string json = JsonConvert.SerializeObject(gameList);
            return new CommandResult(true, Command.List, json, true);
        }
    }
}
