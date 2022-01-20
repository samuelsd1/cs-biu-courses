using System.Collections.Generic;
using MazeLib;
using SearchAlgorithmsLib;

namespace MazeAdapterLib
{
    /// <summary>
    /// a class that is in charge of making the maze
    /// a searchable .by using the object adapter pattern.
    /// </summary>
    /// <seealso cref="Position" />
    public class MazeAdapter : ISearchable<Position>
    {
        private Maze maze;


        /// <summary>
        /// Initializes a new instance of the <see cref="MazeAdapter"/> class.
        /// </summary>
        /// <param name="m">The m.</param>
        public MazeAdapter(Maze m)
        {
            this.maze = m;
        }

        /// <summary>
        /// Gets the initial state.
        /// </summary>
        /// <returns></returns>
        public State<Position> GetInitialState()
        {
            return new State<Position>(maze.InitialPos);
        }

        /// <summary>
        /// Gets the state of the goal.
        /// </summary>
        /// <returns></returns>
        public State<Position> GetGoalState()
        {
            return new State<Position>(maze.GoalPos);
        }

        /// <summary>
        /// Gets all possible states.
        /// </summary>
        /// <param name="s">The s.</param>
        /// <returns></returns>
        public List<State<Position>> GetAllPossibleStates(State<Position> s)
        {
            List<State<Position>> list = new List<State<Position>>();
            int row = s.state.Row;
            int col = s.state.Col;
            int mazeRowSize = maze.Rows;
            int mazeColSize = maze.Cols;

            if ((row + 1 < mazeRowSize) && (maze[row + 1, col] == CellType.Free))
            {
                list.Add(new State<Position>(new Position(row + 1, col)));
            }
            if ((col + 1 < mazeColSize) && (maze[row, col + 1] == CellType.Free))
            {
                list.Add(new State<Position>(new Position(row, col + 1)));
            }
            if ((row - 1 >= 0) && (maze[row - 1, col] == CellType.Free))
            {
                list.Add(new State<Position>(new Position(row - 1, col)));
            }
            if ((col - 1 >= 0) && (maze[row, col - 1] == CellType.Free))
            {
                list.Add(new State<Position>(new Position(row, col - 1)));
            }
            return list;
        }
    }
}