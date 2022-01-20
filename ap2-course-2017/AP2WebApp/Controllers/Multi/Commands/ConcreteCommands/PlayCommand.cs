using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

using SharedData;
using MazeLib;
using WebApp.Models.Requests;
using WebApp.Models.Multi;
using WebApp.Controllers.Multi;

namespace WebApp.Controllers.Commands
{
    /// <summary>
    /// a class that is in charge of executing the
    /// play command
    /// </summary>
    /// <seealso cref="ServerCommand" />
    class PlayCommand : ServerCommand
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="PlayCommand"/> class.
        /// </summary>
        /// <param name="model">The model.</param>
        public PlayCommand(MultiModel model) : base(model) { }

        /// <summary>
        /// Executes the command.
        /// </summary>
        /// <param name="sender">The sender.</param>
        /// <param name="args">The arguments.</param>
        /// <returns></returns>
        public override CommandResult Execute(IClient sender, string[] args)
        {
            if (args.Length != 1)
                return new CommandResult(false, Command.Play, "Usage: play <move>", true);

            Direction dir;
            if (!DirectionParser.TryParse(args[0], out dir))
            {
                return new CommandResult(false, Command.Play, "Bad <move> field.", true);
            }

            PlayRequest request = new PlayRequest(sender, dir);
            try
            {
                _model.Play(request);
                return new CommandResult(true, Command.Play, dir.ToString(), true);
            }
            catch (Exception e)
            {
                return new CommandResult(false, Command.Play, e.Message, true);
            }
        }
    }
}
