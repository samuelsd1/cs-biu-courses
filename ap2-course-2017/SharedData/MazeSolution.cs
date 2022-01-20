using MazeLib;
using Newtonsoft.Json.Linq;
using SearchAlgorithmsLib;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SharedData
{
    /// <summary>
    /// a message that olds a maze solution.
    /// </summary>
    public class MazeSolution
    {
        public int NodesEvaluated { get; private set; }
        public string Name { get; private set; }
        public string Solution { get; private set; }

        /// <summary>
        /// Initializes a new instance of the <see cref="MazePathDetails"/> class.
        /// </summary>
        /// <param name="n">The n.</param>
        /// <param name="s">The s.</param>
        /// <param name="sol">The sol.</param>
        public MazeSolution(int n, string s, Solution<Position> sol)
        {
            NodesEvaluated = n;
            Name = s;
            Solution = ConvertFromStateToString(sol);
        }

        /// <summary>
        /// Prevents a default instance of the 
        /// <see cref="MazeSolution"/> class from being created.
        /// </summary>
        private MazeSolution() { }

        /// <summary>
        /// To the json.
        /// </summary>
        /// <returns></returns>
        public string ToJSON()
        {
            JObject j = new JObject();
            j["NodesEvaluated"] = NodesEvaluated;
            j["Name"] = Name;
            j["Solution"] = Solution;

            return j.ToString();
        }

        /// <summary>
        /// Froms the json.
        /// </summary>
        /// <param name="s">The s.</param>
        /// <returns></returns>
        public static MazeSolution FromJSON(string s)
        {
            MazeSolution ms = new MazeSolution();
            JObject j = JObject.Parse(s);
            ms.Name = (string)j["Name"];
            ms.Solution = (string)j["Solution"];
            ms.NodesEvaluated = (int)j["NodesEvaluated"];

            return ms;
        }

        /// <summary>
        /// Converts from state to string.
        /// </summary>
        /// <param name="sol">The sol.</param>
        /// <returns></returns>
        private static string ConvertFromStateToString(Solution<Position> sol)
        {
            Position curr = sol[0].state;
            Position next;

            int size = sol.Count;
            StringBuilder sb = new StringBuilder();
            
            for (int i = 1; i < size; i++)
            {
                next = sol[i].state;

                if (next.Row == curr.Row - 1)
                {
                    sb.Append("2");
                }
                if (next.Row == curr.Row + 1)
                {
                    sb.Append("3");
                }
                if (next.Col == curr.Col + 1)
                {
                    sb.Append("1");
                }
                if (next.Col == curr.Col - 1)
                {
                    sb.Append("0");
                }
                curr = next;
            }
            return sb.ToString();
        }

    }
}
