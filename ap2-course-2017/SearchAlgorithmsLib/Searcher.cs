using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Priority_Queue;
namespace SearchAlgorithmsLib
{
    /// <summary>
    /// a abstract class that represent
    /// a general searcher.
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <seealso cref="SearchAlgorithmsLib.ISearcher{T}" />
    public abstract class Searcher<T> : ISearcher<T>
    {
        private HashSet<State<T>> closed;
        /// <summary>
        /// Initializes a new instance of the <see cref="Searcher{T}"/> class.
        /// </summary>
        public Searcher()
        {
            closed = new HashSet<State<T>>();
            EvaluatedNodes = 0;
        }

        /// <summary>
        /// Gets the number of nodes evaluated.
        /// </summary>
        /// <returns></returns>
        public int GetNumberOfNodesEvaluated()
        {
            return EvaluatedNodes;
        }

        /// <summary>
        /// Gets the evaluated nodes.
        /// </summary>
        /// <value>
        /// The evaluated nodes.
        /// </value>
        public int EvaluatedNodes
        {
            get;
            private set;
        }

        /// <summary>
        /// Adds to closed.
        /// </summary>
        /// <param name="state">The state.</param>
        public void AddToClosed(State<T> state)
        {
            ++EvaluatedNodes;
            closed.Add(state);
        }

        /// <summary>
        /// Determines whether [is in closed] [the specified state].
        /// </summary>
        /// <param name="state">The state.</param>
        /// <returns>
        ///   <c>true</c> if [is in closed] [the specified state]; otherwise, <c>false</c>.
        /// </returns>
        public bool IsInClosed(State<T> state)
        {
            return closed.Contains(state);
        }

        /// <summary>
        /// Searches the specified searchable.
        /// </summary>
        /// <param name="searchable">The searchable.</param>
        /// <returns></returns>
        public abstract Solution<T> Search(ISearchable<T> searchable);
    }
}
