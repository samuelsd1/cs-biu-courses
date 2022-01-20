using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SearchAlgorithmsLib
{
    /// <summary>
    /// an interface that defines what is nedeed
    /// from a searcher.
    /// </summary>
    /// <typeparam name="T"></typeparam>
    public interface ISearcher<T>
    {
        Solution<T> Search(ISearchable<T> searchable);

        int GetNumberOfNodesEvaluated();
    }
}
