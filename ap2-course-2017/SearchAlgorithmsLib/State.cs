using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SearchAlgorithmsLib
{
    /// <summary>
    /// a class that represent a state in a
    /// world.according to the world we in.
    /// </summary>
    /// <typeparam name="T"></typeparam>
    public class State<T>
    {
        public T state { get; protected set; }
        public float Cost { get; set; }
        public State<T> CameFromState { get; set; }

        /// <summary>
        /// Initializes a new instance of the <see cref="State{T}"/> class.
        /// </summary>
        /// <param name="state">The state.</param>
        public State(T state)
        {
            this.state = state;
        }

        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        /// <returns>
        /// A hash code for this instance,
        ///  suitable for use in hashing algorithms and data structures like a hash table. 
        /// </returns>
        public override int GetHashCode()
        {
            return state.GetHashCode();
        }

        /// <summary>
        /// Determines whether the specified <see cref="System.Object" />, 
        /// is equal to this instance.
        /// </summary>
        /// <param name="obj">The <see cref="System.Object" /> 
        /// to compare with this instance.</param>
        /// <returns>
        ///   <c>true</c> if the specified <see cref="System.Object" /> 
        /// is equal to this instance; otherwise, <c>false</c>.
        /// </returns>
        public override bool Equals(object obj)
        {
            return Equals(obj as State<T>);
        }

        /// <summary>
        /// Equalses the specified object.
        /// </summary>
        /// <param name="obj">The object.</param>
        /// <returns></returns>
        public bool Equals(State<T> obj)
        {
            if (obj == null)
                return false;
            return state.Equals(obj.state);
        }

        /// <summary>
        /// Returns a <String /> that represents this instance.
        /// </summary>
        /// <returns>
        /// A <see cref="System.String" /> that represents this instance.
        /// </returns>
        public override string ToString()
        {
            return state.ToString();
        }
    }
}