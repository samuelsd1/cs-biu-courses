using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace WebApp.Controllers.Multi
{
    /// <summary>
    /// the class that olding the players
    /// </summary>
    public class ClientContainer
    {
        /// <summary>
        /// The i dto user
        /// </summary>
        private Dictionary<string, string> IDtoUser;
        /// <summary>
        /// The user to client
        /// </summary>
        private Dictionary<string, SignalRWebClient> userToClient;

        /// <summary>
        /// Initializes a new instance of the <see cref="ClientContainer"/> class.
        /// </summary>
        public ClientContainer()
        {
            IDtoUser = new Dictionary<string, string>();
            userToClient = new Dictionary<string, SignalRWebClient>();
        }

        /// <summary>
        /// Adds the specified username.
        /// </summary>
        /// <param name="username">The username.</param>
        /// <param name="id">The identifier.</param>
        public void Add(string username, string id)
        {
            IDtoUser.Add(id, username);
            userToClient.Add(username, new SignalRWebClient(username, id));
        }

        /// <summary>
        /// Gets the client by identifier.
        /// </summary>
        /// <param name="id">The identifier.</param>
        /// <returns></returns>
        public SignalRWebClient GetClientByID(string id)
        {
            if (!ContainsByID(id))
                return null;
            string username = IDtoUser[id];
            return userToClient[username];
        }

        /// <summary>
        /// Determines whether [contains by identifier] [the specified identifier].
        /// </summary>
        /// <param name="id">The identifier.</param>
        /// <returns>
        ///   <c>true</c> if [contains by identifier] [the specified identifier]; otherwise, <c>false</c>.
        /// </returns>
        public bool ContainsByID(string id)
        {
            return IDtoUser.ContainsKey(id);
        }

        /// <summary>
        /// Determines whether [contains by username] [the specified username].
        /// </summary>
        /// <param name="username">The username.</param>
        /// <returns>
        ///   <c>true</c> if [contains by username] [the specified username]; otherwise, <c>false</c>.
        /// </returns>
        public bool ContainsByUsername(string username)
        {
            return userToClient.ContainsKey(username) && IDtoUser.ContainsValue(username);
        }

        /// <summary>
        /// Updates the identifier.
        /// </summary>
        /// <param name="username">The username.</param>
        /// <param name="newID">The new identifier.</param>
        /// <returns></returns>
        public bool UpdateID(string username, string newID)
        {
            if (!ContainsByUsername(username))
                return false;

            string oldID = userToClient[username].ID;
            IDtoUser.Remove(oldID);
            IDtoUser.Add(newID, username);

            userToClient[username].ID = newID;

            return true;
        }
    }
}