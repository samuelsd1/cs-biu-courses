using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using WebApp.Models.Multi;

namespace WebApp.Controllers.Multi
{
    /// <summary>
    /// the client of signalr type
    /// </summary>
    /// <seealso cref="WebApp.Models.Multi.IClient" />
    public class SignalRWebClient : IClient
    {
        /// <summary>
        /// Gets the name of the user.
        /// </summary>
        /// <value>
        /// The name of the user.
        /// </value>
        public string UserName { get; }
        /// <summary>
        /// Gets or sets the identifier.
        /// </summary>
        /// <value>
        /// The identifier.
        /// </value>
        public string ID { get; set; }

        /// <summary>
        /// Initializes a new instance of the <see cref="SignalRWebClient"/> class.
        /// </summary>
        /// <param name="userName">Name of the user.</param>
        /// <param name="id">The identifier.</param>
        public SignalRWebClient(string userName, string id)
        {
            this.UserName = userName;
            this.ID = id;
        }

        /// <summary>
        /// Determines whether the specified <see cref="System.Object" />, is equal to this instance.
        /// </summary>
        /// <param name="obj">The <see cref="System.Object" /> to compare with this instance.</param>
        /// <returns>
        ///   <c>true</c> if the specified <see cref="System.Object" /> is equal to this instance; otherwise, <c>false</c>.
        /// </returns>
        public override bool Equals(object obj)
        {
            return Equals(obj as SignalRWebClient);
        }

        /// <summary>
        /// Equalses the specified client.
        /// </summary>
        /// <param name="client">The client.</param>
        /// <returns></returns>
        public bool Equals(SignalRWebClient client)
        {
            if (client == null)
                return false;
            return this.UserName.Equals(client.UserName);
        }

        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        /// <returns>
        /// A hash code for this instance, suitable for use in hashing algorithms and data structures like a hash table. 
        /// </returns>
        public override int GetHashCode()
        {
            return UserName.GetHashCode();
        }
    }
}