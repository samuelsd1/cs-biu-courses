using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace WebApp.DataObjects
{
    /// <summary>
    /// represent a solve request to server
    /// </summary>
    public class SolveInfo
    {
        /// <summary>
        /// Gets or sets the name.
        /// </summary>
        /// <value>
        /// The name.
        /// </value>
        public String Name { get; set; }
        /// <summary>
        /// Gets or sets the algo.
        /// </summary>
        /// <value>
        /// The algo.
        /// </value>
        public String Algo { get; set; }
    }
}