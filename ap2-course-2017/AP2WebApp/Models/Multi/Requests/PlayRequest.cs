using MazeLib;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using WebApp.Models.Multi;

namespace WebApp.Models.Requests
{
    /// <summary>
    /// a class that represente a request from the 
    /// client to play a move.
    /// </summary>
    /// <seealso cref="IRequest" />
    public class PlayRequest : IRequest
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="PlayRequest"/> class.
        /// </summary>
        /// <param name="client">The player.</param>
        /// <param name="move">The move.</param>
        public PlayRequest(IClient client, Direction move)
        {
            this.Client = client;
            this.Move = move;
        }

        /// <summary>
        /// Gets the player.
        /// </summary>
        /// <value>
        /// The player.
        /// </value>
        public IClient Client
        {
            get;
        }

        /// <summary>
        /// Gets the move.
        /// </summary>
        /// <value>
        /// The move.
        /// </value>
        public Direction Move
        {
            get;
        }
    }
}
