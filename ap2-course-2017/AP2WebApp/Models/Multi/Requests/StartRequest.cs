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
    /// client to start a new multiplayer game
    /// </summary>
    /// <seealso cref="IRequest" />
    public class StartRequest : IRequest
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="StartRequest"/> class.
        /// </summary>
        /// <param name="client">The starter.</param>
        /// <param name="details">The details.</param>
        public StartRequest(IClient client, GenerateRequest details)
        {
            this.Client = client;
            this.MazeDetails = details;
        }

        /// <summary>
        /// Gets the starter.
        /// </summary>
        /// <value>
        /// The starter.
        /// </value>
        public IClient Client
        {
            get;
        }

        /// <summary>
        /// Gets the maze details.
        /// </summary>
        /// <value>
        /// The maze details.
        /// </value>
        public GenerateRequest MazeDetails
        {
            get;
        }
    }
}
