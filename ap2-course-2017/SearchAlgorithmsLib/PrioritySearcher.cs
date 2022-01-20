using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Priority_Queue;

namespace SearchAlgorithmsLib
{
    /// <summary>
    ///  an abstract class that represent a search
    /// algo that is using a queue.
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <seealso cref="SearchAlgorithmsLib.Searcher{T}" />
    public abstract class PrioritySearcher<T> : Searcher<T>
    {
        private SimplePriorityQueue<State<T>> open;

        /// <summary>
        /// Initializes a new instance of the <see cref="PrioritySearcher{T}"/> class.
        /// </summary>
        public PrioritySearcher() : base()
        {
            open = new SimplePriorityQueue<State<T>>();
        }

        /// <summary>
        /// Adds to open.
        /// </summary>
        /// <param name="s">The s.</param>
        protected void AddToOpen(State<T> s)
        {
            open.Enqueue(s, s.Cost);
        }

        /// <summary>
        /// Dequeues this instance.
        /// </summary>
        /// <returns></returns>
        protected State<T> Dequeue()
        {
            return open.Dequeue();
        }

        /// <summary>
        /// Removes from open.
        /// </summary>
        /// <param name="s">The s.</param>
        protected void RemoveFromOpen(State<T> s)
        {
            open.Remove(s);
        }

        /// <summary>
        /// Determines whether [is in open] [the specified s].
        /// </summary>
        /// <param name="s">The s.</param>
        /// <returns>
        ///   <c>true</c> if [is in open] [the specified s]; otherwise, <c>false</c>.
        /// </returns>
        protected bool IsInOpen(State<T> s)
        {
            
            return open.Contains(s);
        }

        /// <summary>
        /// Gets the size of the open.
        /// </summary>
        /// <value>
        /// The size of the open.
        /// </value>
        protected int OpenSize
        {
            get { return open.Count; }
        }
    }
}
