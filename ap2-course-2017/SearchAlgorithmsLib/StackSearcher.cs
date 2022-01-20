using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SearchAlgorithmsLib
{
    /// <summary>
    /// an abstract class that represent a search
    /// algo that is using a stack.
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <seealso cref="SearchAlgorithmsLib.Searcher{T}" />
    public abstract class StackSearcher<T> : Searcher<T>
    {
        private Stack<State<T>> open;

        /// <summary>
        /// Initializes a new instance of the <StackSearcher{T}/> class.
        /// </summary>
        public StackSearcher()
        {
            this.open = new Stack<State<T>>();
        }

        /// <summary>
        /// Pushes the specified s.
        /// </summary>
        /// <param name="s">The s.</param>
        protected void Push(State<T> s)
        {
            open.Push(s);
        }

        /// <summary>
        /// Pops this instance.
        /// </summary>
        /// <returns></returns>
        protected State<T> Pop()
        {
            return open.Pop();
        }

        /// <summary>
        /// Gets the count.
        /// </summary>
        /// <value>
        /// The count.
        /// </value>
        protected int Count
        {
            get { return open.Count; }
        }
    }
}
