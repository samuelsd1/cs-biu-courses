using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace WebApp.DataObjects
{
    /// <summary>
    /// the user logic details
    /// </summary>
    public class UserLogin
    {
        /// <summary>
        /// Gets or sets the name of the user.
        /// </summary>
        /// <value>
        /// The name of the user.
        /// </value>
        public String UserName { get; set; }
        /// <summary>
        /// Gets or sets the password.
        /// </summary>
        /// <value>
        /// The password.
        /// </value>
        public String Password { get; set; }
    }
}