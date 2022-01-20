using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SearchAlgorithmsLib
{
    /// <summary>
    /// a class that old's the solution of a
    /// search on a searchable object. 
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <seealso cref="State{T}" />
    public class Solution<T> : List<State<T>>
    {
        public int NodesEvaluated { get; }
        /// <summary>
        /// Initializes a new instance of the <see cref="Solution{T}"/> class.
        /// </summary>
        /// <param name="n">The n.</param>
        public Solution(int n)
        {
            NodesEvaluated = n;
        }
    }
}