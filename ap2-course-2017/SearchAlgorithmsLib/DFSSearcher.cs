using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SearchAlgorithmsLib
{
    /// <summary>
    /// implement the stack searcer.
    /// search with dfs algo.
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <seealso cref="SearchAlgorithmsLib.StackSearcher{T}" />
    public class DFSSearcher<T> : StackSearcher<T>
    {
        /// <summary>
        /// Searches the specified searchable.
        /// </summary>
        /// <param name="searchable">The searchable.</param>
        /// <returns></returns>
        public override Solution<T> Search(ISearchable<T> searchable)
        {
            State<T> v = searchable.GetInitialState();
            v.CameFromState = null;
            
            Push(v);
            
            bool found = false;
            
            // while stack not empty
            while (Count > 0)
            {
                v = Pop();
                if (v.Equals(searchable.GetGoalState())){
                    found = true;
                    break;
                }

                // if not discovered
                if(!IsInClosed(v))
                {
                    // add the vertex to the handeled set
                    AddToClosed(v);

                    // foreach neighbor w of v
                    foreach (State<T> w in searchable.GetAllPossibleStates(v))
                    {
                        // if the vertex was already handeled, do not touch it.
                        if (IsInClosed(w))
                            continue;

                        w.CameFromState = v;
                        Push(w);
                    }
                }
            }
            
            // backtrace the solution (if found)
            Solution<T> sol = new Solution<T>(base.EvaluatedNodes);
            if (found)
            {
                // then current = goal state
                while(v != null)
                {
                    sol.Insert(0, v); 
                    v = v.CameFromState;
                }
            }
            return sol;
        }
    }
}
