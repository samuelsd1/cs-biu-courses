using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WebApp.Models.Requests
{
    /// <summary>
    /// a class that represente a request from the
    /// client to generate a maze
    /// </summary>
    /// <seealso cref="IRequest" />
    public class GenerateRequest : IRequest
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="GenerateRequest"/> class.
        /// </summary>
        /// <param name="name">The name.</param>
        /// <param name="rows">The rows.</param>
        /// <param name="cols">The cols.</param>
        public GenerateRequest(string name, int rows, int cols)
        {
            this.MazeName = name;
            this.Rows = rows;
            this.Columns = cols;
        }

        /// <summary>
        /// Gets the name of the maze.
        /// </summary>
        /// <value>
        /// The name of the maze.
        /// </value>
        public string MazeName { get; }

        /// <summary>
        /// Gets the rows.
        /// </summary>
        /// <value>
        /// The rows.
        /// </value>
        public int Rows { get; }

        /// <summary>
        /// Gets the columns.
        /// </summary>
        /// <value>
        /// The columns.
        /// </value>
        public int Columns { get; }
    }
}