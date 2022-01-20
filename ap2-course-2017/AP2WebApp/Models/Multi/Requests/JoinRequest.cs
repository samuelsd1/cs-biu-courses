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
    /// client to join a given game
    /// </summary>
    /// <seealso cref="global::WebApp.Models.Requests.IRequest" />
    public class JoinRequest : IRequest
    {

        /// <summary>
        /// Initializes a new instance of the <see cref="JoinRequest"/> class.
        /// </summary>
        /// <param name="client">The client.</param>
        /// <param name="name">The name.</param>
        public JoinRequest(IClient client, string name)
        {
            this.Client = client;
            this.GameName = name;
        }

        /// <summary>
        /// Gets the client.
        /// </summary>
        /// <value>
        /// The client.
        /// </value>
        public IClient Client
        {
            get;
        }

        /// <summary>
        /// Gets the name of the game.
        /// </summary>
        /// <value>
        /// The name of the game.
        /// </value>
        public string GameName
        {
            get;
        }

    }
}
