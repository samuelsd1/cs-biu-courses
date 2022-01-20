using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using MazeLib;
using SearchAlgorithmsLib;

namespace WebApp.Models.Requests
{
    /// <summary>
    /// a class that represent a request from the 
    /// client to solve a maze
    /// </summary>
    /// <seealso cref="IRequest" />
    public class SolveRequest : IRequest
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="SolveRequest"/> class.
        /// </summary>
        /// <param name="name">The name.</param>
        /// <param name="algorithm">The algorithm.</param>
        public SolveRequest(string name, byte algorithm)
        {
            this.MazeName = name;
            this.Algorithm = algorithm;
        }


        /// <summary>
        /// Gets the name of the maze.
        /// </summary>
        /// <value>
        /// The name of the maze.
        /// </value>
        public string MazeName
        {
            get;
        }

        /// <summary>
        /// Gets the algorithm.
        /// </summary>
        /// <value>
        /// The algorithm.
        /// </value>
        public byte Algorithm
        {
            get;
        }
    }
}
