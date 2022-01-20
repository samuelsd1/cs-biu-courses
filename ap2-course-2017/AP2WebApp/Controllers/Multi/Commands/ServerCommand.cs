using SharedData;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using WebApp.Models.Multi;

namespace WebApp.Controllers.Commands
{
    /// <summary>
    /// the server command.
    /// implements Icommand.
    /// olding the common between the command-the model.
    /// </summary>
    /// <seealso cref="ICommand" />
    abstract class ServerCommand : ICommand
    {
        protected MultiModel _model;
        /// <summary>
        /// Initializes a new instance of the <see cref="ServerCommand"/> class.
        /// </summary>
        /// <param name="model">The model.</param>
        public ServerCommand(MultiModel model)
        {
            _model = model;
        }

        /// <summary>
        /// Executes the specified sender.
        /// </summary>
        /// <param name="sender">The sender.</param>
        /// <param name="args">The arguments.</param>
        /// <returns></returns>
        public abstract CommandResult Execute(IClient sender, string[] args);
    }
}
