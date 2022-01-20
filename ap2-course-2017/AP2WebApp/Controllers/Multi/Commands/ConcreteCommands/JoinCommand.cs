using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

using SharedData;
using WebApp.Models.Requests;
using WebApp.Models.Multi;

namespace WebApp.Controllers.Commands
{
    /// <summary>
    /// a class that is in charge of executing the
    /// join command
    /// </summary>
    /// <seealso cref="ServerCommand" />
    class JoinCommand : ServerCommand
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="JoinCommand" /> class.
        /// </summary>
        /// <param name="model">The model.</param>
        public JoinCommand(MultiModel model) : base(model) { }

        /// <summary>
        /// Executes the command.
        /// </summary>
        /// <param name="sender">The sender.</param>
        /// <param name="args">The arguments.</param>
        /// <returns></returns>
        public override CommandResult Execute(IClient sender, string[] args)
        {
            // TODO: decide when to keep connection, when to close it.
            if (args.Length != 1)
                return new CommandResult(false, Command.Join, "Usage: join <name>", true);

            JoinRequest request = new JoinRequest(sender, args[0]);
            try
            {
                _model.JoinGame(request);
                return new CommandResult(true, Command.Join, "Successfully connected, starting the game...", true);
            }
            catch (Exception e)
            {
                return new CommandResult(false, Command.Join, e.Message, true);
            }
        }
    }
}
