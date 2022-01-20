using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SearchAlgorithmsLib
{
    public class BFSSearcher<T> : PrioritySearcher<T>
    {

       
        /// <summary>
        /// Initializes a new instance of the <see cref="BFSSearcher{T}"/> class.
        /// </summary>
        public BFSSearcher() : base()
        {
        }

       
        /// <summary>
        /// the override function that finds the solution
        /// to the search problem using best first search
        /// </summary>
        /// <param name="searchable">The searchable.</param>
        /// <returns></returns>
    public override Solution<T> Search(ISearchable<T> searchable)
        {
            State<T> v = searchable.GetInitialState();
            v.CameFromState = null;
            AddToOpen(v);

            bool found = false;

            while (OpenSize > 0)
            {
                //remove the one with highest priorty
                v = Dequeue();
                //mark as item in progress
                AddToClosed(v);

                //handle the neighbors
                // if it is the solution
                if (v.Equals(searchable.GetGoalState()))
                {
                    found = true;
                    break;
                }

                //get and update neighbors
                UpdateNeighbor(v, searchable.GetAllPossibleStates(v));
            }

            // backtrace the solution (if found)
            Solution<T> sol = new Solution<T>(base.EvaluatedNodes);
            if (found)
            {
                // then current = goal state
                while (v != null)
                {
                    sol.Insert(0, v);
                    v = v.CameFromState;
                }
            }
            return sol;
        }

        private void UpdateNeighbor(State<T> curr, List<State<T>> neighbors)
        {
            // for each neighbor of curr
            foreach (State<T> neighbor in neighbors)
            {
                // if not in open and not in closed
                if ((!IsInOpen(neighbor)) && (!IsInClosed(neighbor)))
                {
                    neighbor.CameFromState = curr;
                    neighbor.Cost = curr.Cost + 1;
                    AddToOpen(neighbor);
                }
                // if found better way
                else if (IsInOpen(neighbor))
                {
                    if (neighbor.Cost > curr.Cost + 1) 
                    {
                        RemoveFromOpen(neighbor);
                        neighbor.CameFromState = curr;
                        neighbor.Cost = curr.Cost + 1;
                        AddToOpen(neighbor);
                    }
                    /*else  ///todo !!!!!
                    {
                        AddToOpen(neighbor);
                    }*/
                }
            }
        }
    }
}