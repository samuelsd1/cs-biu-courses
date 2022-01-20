using MazeLib;
using SharedData;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using WebApp.Controllers.Commands;
using WebApp.Models.Multi;
using WebApp.Models.Multi.Game;

namespace WebApp.Controllers.Multi
{
    /// <summary>
    /// the controller part of the mvc pattern.
    /// </summary>
    class Controller : IGameEventHandler
    {
        private MultiModel model;
        private IClientNotifier notifier;
        private Dictionary<string, ICommand> commandSet;

        /// <summary>
        /// Initializes a new instance of the <see cref="Controller"/> class.
        /// </summary>
        public Controller(IClientNotifier notifier)
        {
            this.model = new MultiModel(this);
            this.notifier = notifier;

            //initialize the dictionary
            this.commandSet = new Dictionary<string, ICommand>();
            this.commandSet.Add("start", new StartCommand(model));
            this.commandSet.Add("list", new ListCommand(model));
            this.commandSet.Add("join", new JoinCommand(model));
            this.commandSet.Add("play", new PlayCommand(model));
            this.commandSet.Add("close", new CloseCommand(model));
        }

        /// <summary>
        /// Executes the command.
        /// </summary>
        /// <param name="command">The command.</param>
        /// <param name="sender">The sender.</param>
        /// <returns></returns>
        public CommandResult ExecuteCommand(string command, IClient sender)
        {
            string[] args = command.Split(' ');
            string commandName = args[0];
            args = args.Skip(1).ToArray();
            if (!commandSet.ContainsKey(commandName))
            {
                return new CommandResult(false, Command.Undefined, "Unknown command", true);
            }
            return commandSet[commandName].Execute(sender, args);
        }

        /// <summary>
        /// Handles the game started.
        /// </summary>
        /// <param name="game">The game.</param>
        public void HandleGameStarted(MazeGame game)
        {
            string json = game.Maze.ToJSON();
            Notification notif = new Notification(Notification.Type.GameStarted, json);
            List<IClient> clients = game.GetAllClients();
            foreach (IClient client in clients)
            {
                notifier.NotifyClient(client, notif);
            }
        }

        /// <summary>
        /// Handles the game over.
        /// </summary>
        /// <param name="game">The game.</param>
        /// <param name="args">The <see cref="GameOverEventArgs"/>
        ///  instance containing the event data.</param>
        public void HandleGameOver(MazeGame game, GameOverEventArgs args)
        {
            List<IClient> clients = game.GetAllClients();
            foreach (IClient client in clients)
            {
                string message;
                if (args.Winner == null)
                {
                    message = "The game was closed by your opponents.";
                }
                else
                {
                    message = client.Equals(args.Winner) ? "You Win!" : "You Lost!";
                }
                Notification notif = new Notification(Notification.Type.GameOver, message);
                notifier.NotifyClient(client, notif);
            }
        }

        /// <summary>
        /// Handles the player moved.
        /// </summary>
        /// <param name="game">The game.</param>
        /// <param name="player">The player.</param>
        /// <param name="d">The d.</param>
        public void HandlePlayerMoved(MazeGame game, IClient player, Direction d)
        {
            MoveUpdate update = new MoveUpdate(game.Name, d);
            Notification notif = new Notification(Notification.Type.PlayerMoved, update.ToJSON());
            List<IClient> clients = game.GetAllClients();
            foreach (IClient client in clients)
            {
                // do not notify the player which moved.
                if (client.Equals(player))
                    continue;

                //notify the client.
                notifier.NotifyClient(client, notif);
            }
        }
    }
}