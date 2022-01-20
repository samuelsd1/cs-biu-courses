using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using Microsoft.AspNet.SignalR;
using System.Collections.Concurrent;
using Microsoft.AspNet.SignalR.Hubs;
using WebApp.Models.Multi;
using MazeLib;
using WebApp.Models.Multi.Game;
using SharedData;
using WebApp.Controllers.Commands;
using WebApp.Controllers.Multi;
using MazeAdapterLib;
using MazeGeneratorLib;
using Newtonsoft.Json.Linq;

namespace WebApp.Controllers.Multi
{

    /// <summary>
    /// the hub for 2-way communication
    /// </summary>
    /// <seealso cref="Microsoft.AspNet.SignalR.Hub" />
    /// <seealso cref="WebApp.Controllers.Multi.IClientNotifier" />
    [HubName("MultiPlayerHub")]
    public class MultiPlayerHub : Hub, IClientNotifier
    {
        /// <summary>
        /// The container
        /// </summary>
        private static ClientContainer container = new ClientContainer();

        /// <summary>
        /// The controller
        /// </summary>
        private static Controller controller;

        /// <summary>
        /// Subscribes the specified username.
        /// </summary>
        /// <param name="username">The username.</param>
        public void Subscribe(string username)
        {
            if (controller == null)
            {
                controller = new Controller(this);
            }

            string id = Context.ConnectionId;
            if (container.ContainsByUsername(username))
            {
                container.UpdateID(username, id);
            }
            else
            {
                container.Add(username, id);
            }
        }

        /// <summary>
        /// Executes the specified command.
        /// </summary>
        /// <param name="command">The command.</param>
        public void Execute(string command)
        {
            string id = Context.ConnectionId;

            SignalRWebClient sender = container.GetClientByID(id);
            if (sender == null)
                return;

            CommandResult res = controller.ExecuteCommand(command, sender);
            SendResult(sender, res);
        }

        /// <summary>
        /// Sends the result.
        /// </summary>
        /// <param name="c">The c.</param>
        /// <param name="res">The resource.</param>
        private void SendResult(SignalRWebClient c, CommandResult res)
        {
            Clients.Client(c.ID).CommandResult(res.ToJSON());
        }

        /// <summary>
        /// Notifies the client.
        /// </summary>
        /// <param name="c">The c.</param>
        /// <param name="n">The n.</param>
        public void NotifyClient(IClient c, Notification n)
        {
            SignalRWebClient client = c as SignalRWebClient;
            switch (n.NotificationType)
            {
                case Notification.Type.GameStarted:
                    //Maze maze = Maze.FromJSON(n.Data);
                    Clients.Client(client.ID).GameStarted(n.Data);
                    break;
                case Notification.Type.GameOver:
                    // n.data is message from the server.
                    // if the message contains the keyword Win, then its the winning player.
                    // i know, this is a dirty workaround, but it works and im too busy
                    // to rewrite it.
                    if (n.Data.Contains("Win"))
                    {
                        PlayerController.AddWin(client.UserName);
                    }
                    else if(n.Data.Contains("Lost"))
                    {
                        PlayerController.AddLoss(client.UserName);
                    }
                    Clients.Client(client.ID).GameOver(n.Data);
                    break;
                case Notification.Type.PlayerMoved:
                    //MoveUpdate mu = MoveUpdate.FromJSON(n.Data);
                    Clients.Client(client.ID).PlayerMoved(n.Data);
                    break;
                default:
                    break;
            }
        }
    }
}