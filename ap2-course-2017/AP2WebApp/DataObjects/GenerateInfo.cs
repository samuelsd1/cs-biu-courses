using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace WebApp.DataObjects
{
    /// <summary>
    /// represent a generate request to server
    /// </summary>
    public class GenerateInfo
    {
        /// <summary>
        /// Gets or sets the name.
        /// </summary>
        /// <value>
        /// The name.
        /// </value>
        public String Name { get; set; }
        /// <summary>
        /// Gets or sets the rows.
        /// </summary>
        /// <value>
        /// The rows.
        /// </value>
        public String Rows { get; set; }
        /// <summary>
        /// Gets or sets the cols.
        /// </summary>
        /// <value>
        /// The cols.
        /// </value>
        public String Cols { get; set; }
    }
}