using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

using SharedData;
using WebApp.Models.Multi;
using WebApp.Models.Requests;

namespace WebApp.Controllers.Commands
{
    /// <summary>
    /// a class that is in charge of executing the
    /// start command
    /// </summary>
    /// <seealso cref="ServerCommand" />
    class StartCommand : ServerCommand
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="StartCommand"/> class.
        /// </summary>
        /// <param name="model">The model.</param>
        public StartCommand(MultiModel model) : base(model) { }

        /// <summary>
        /// Executes the command.
        /// </summary>
        /// <param name="sender">The sender.</param>
        /// <param name="args">The arguments.</param>
        /// <returns></returns>
        public override CommandResult Execute(IClient sender, string[] args)
        {
            // generate command expects exactly 3 parameters
            
            if (args.Length != 3)
                return new CommandResult(false, Command.Start, "Usage: start <name> <rows> <cols>", true);

            string name;
            int rows, cols;

            name = args[0];
            if (!int.TryParse(args[1], out rows))
                return new CommandResult(false, Command.Start, "Bad <rows> field.", true);

            if (!int.TryParse(args[2], out cols))
                return new CommandResult(false, Command.Start, "Bad <cols> field.", true);

            StartRequest request = new StartRequest(sender, new GenerateRequest(name, rows, cols));

            try
            {
                _model.CreateGame(request);
                return new CommandResult(true, Command.Start, "A game was created, please wait for your opponent to connect...", true);
            }
            catch (Exception e)
            {
                return new CommandResult(false, Command.Start, e.Message, true);
            }
        }
    }
}
