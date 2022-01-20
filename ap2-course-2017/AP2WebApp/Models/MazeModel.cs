using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Web;
using MazeAdapterLib;
using MazeGeneratorLib;
using MazeLib;
using SearchAlgorithmsLib;

namespace WebApp.Models
{
    /// <summary>
    /// handle the maze logic
    /// </summary>
    public class MazeModel
    {
        /// <summary>
        /// Gets or sets the generator.
        /// </summary>
        /// <value>
        /// The generator.
        /// </value>
        public DFSMazeGenerator Generator { get; set; }
        /// <summary>
        /// Gets or sets the game dictionary.
        /// </summary>
        /// <value>
        /// The game dictionary.
        /// </value>
        public static Dictionary<String, Maze> GameDictionary { get; set; } = new Dictionary<string, Maze>();

        /// <summary>
        /// Initializes a new instance of the <see cref="MazeModel"/> class.
        /// </summary>
        public MazeModel()
        {
            Generator = new DFSMazeGenerator();
        }

        /// <summary>
        /// Gets the new model.
        /// </summary>
        /// <param name="Name">The name.</param>
        /// <param name="rows">The rows.</param>
        /// <param name="cols">The cols.</param>
        /// <returns></returns>
        public Maze GetNewModel(String Name, int rows, int cols)
        {
            Maze m = Generator.Generate(rows, cols);
            m.Name = Name;
            if (GameDictionary.ContainsKey(Name))
            {
                return null;
            }
            else
            {
                GameDictionary.Add(Name, m);
            }
            return m;
        }

        /// <summary>
        /// Solves the game.
        /// </summary>
        /// <param name="s">The s.</param>
        /// <param name="algo">The algo.</param>
        /// <returns></returns>
        public String SolveGame(String s, String algo)
        {
            if (!GameDictionary.ContainsKey(s))
            {
                return "not such maze";
            }
            Maze maze = GameDictionary[s];
            ISearchable<Position> searchable = new MazeAdapter(maze);
            ISearcher<Position> searcher;
            if (algo.Equals("BFS"))
            {
                searcher = new BFSSearcher<Position>();
            }
            else
            {
                searcher = new DFSSearcher<Position>();
            }

            Solution<Position> solution = searcher.Search(searchable);

            return ConvertToString(solution);
        }

        /// <summary>
        /// Converts to string.
        /// </summary>
        /// <param name="sol">The sol.</param>
        /// <returns></returns>
        private String ConvertToString(Solution<Position> sol)
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