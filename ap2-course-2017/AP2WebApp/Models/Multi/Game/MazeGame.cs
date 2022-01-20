using MazeLib;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WebApp.Models.Multi.Game
{
    /// <summary>
    /// implements the specific logig of a
    /// multiplayer game.
    /// </summary>
    public class MazeGame
    {
        public event GameStarted onGameStarted;
        public event GameOver onGameOver;
        public event PlayerMoved onPlayerMoved;

        public Maze Maze { get; }
        public string Name
        {
            get { return this.Maze.Name; }
        }
        public bool Started { get; private set; }
        public bool Finished { get; private set; }
        private List<MazeGamePlayer> players;

        #region Ctors

        /// <summary>
        /// Initializes a new instance of the <see cref="MazeGame"/> class.
        /// </summary>
        /// <param name="maze">The maze.</param>
        public MazeGame(Maze maze)
        {
            this.Maze = maze;
            this.Started = false;
            this.Finished = false;
            this.players = new List<MazeGamePlayer>();
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MazeGame"/> class.
        /// </summary>
        /// <param name="maze">The maze.</param>
        /// <param name="name">The name.</param>
        public MazeGame(Maze maze, string name)
            : this(maze)
        {
            this.Maze.Name = name;
        }

        #endregion
        
        /// <summary>
        /// Starts this instance.
        /// </summary>
        private void Start()
        {
            if (Started)
                return;
            Started = true;
            onGameStarted(this);
        }

        /// <summary>
        /// Called when a player won the match
        /// </summary>
        /// <param name="winner"></param>
        private void Finish(MazeGamePlayer winner)
        {
            if (!Started || Finished)
                return;

            Finished = true;
            onGameOver(this, new GameOverEventArgs(winner.Client));
        }

        /// <summary>
        /// called when all players left the match, closes the game,
        /// and calles the GameOver event.
        /// </summary>
        private void Close()
        {
            if (!Started || Finished)
                return;
            Finished = true;
            onGameOver(this, new GameOverEventArgs());
        }

        /// <summary>
        /// Adds a player to the game.
        /// </summary>
        /// <param name="client"></param>
        public void AddPlayer(IClient client)
        {
            if (Started)
                throw new InvalidOperationException("Game already started");

            if (ContainsClient(client))
                throw new InvalidOperationException("Player already exists");

            MazeGamePlayer player = new MazeGamePlayer(client, Maze.InitialPos);
            players.Add(player);

            if (players.Count >= 2)
                Start();
        }

        /// <summary>
        /// removes a client from the game.
        /// If less than 2 clients left, closes the game.
        /// </summary>
        /// <param name="client"></param>
        public void RemovePlayer(IClient client)
        {
            MazeGamePlayer player = GetPlayerByClient(client);
            if (player == null)
                throw new InvalidOperationException(string.Format("You are not in the game \"{0}\"", Name));

            players.Remove(player);

            if (players.Count <= 1)
                Close();
        }

        /// <summary>
        /// returns all the clients in the game.
        /// </summary>
        /// <returns></returns>
        public List<IClient> GetAllClients()
        {
            return this.players.Select(x => x.Client).ToList();
        }

        /// <summary>
        /// Determines whether a player
        /// can move to the specified p.
        /// </summary>
        /// <param name="p">The p.</param>
        /// <returns>
        ///   <c>true</c> if this instance [can move to] the specified p;
        ///  otherwise, <c>false</c>.
        /// </returns>
        private bool CanMoveTo(Position p)
        {
            // TODO check if rows and cols are zero based or 1 based, fix the definition.
            int row = p.Row;
            int col = p.Col;
            if (row < 0 || row >= Maze.Rows)
                return false;
            if (col < 0 || col >= Maze.Cols)
                return false;
            return Maze[row, col] == CellType.Free;
        }

        /// <summary>
        /// Plays the move.
        /// </summary>
        /// <param name="client">The client that moves.</param>
        /// <param name="d">The direction of the movement.</param>
        /// <returns></returns>
        public void PlayMove(IClient client, Direction d)
        {
            if (!Started)
                throw new InvalidOperationException(string.
                    Format("The game \"{0}\" did not start yet.", Name));

            if (Finished)
                throw new InvalidOperationException(string.
                    Format("The game \"{0}\" is over.", Name));

            MazeGamePlayer player = GetPlayerByClient(client);
            if (player == null)
                throw new InvalidOperationException(string.
                    Format("You are not in the game \"{0}\"", Name));

            Position estimated = player.GetEstimatedPos(d);
            if (!CanMoveTo(estimated))
                throw new InvalidOperationException("You can not move to that direction.");

            player.Position = estimated;
            onPlayerMoved(this, player.Client, d);

            CheckWin(player);
        }

        /// <summary>
        /// Checks the win.
        /// </summary>
        /// <param name="p">The p.</param>
        private void CheckWin(MazeGamePlayer p)
        {
            if (p.Position.Equals(Maze.GoalPos))
            {
                onGameOver(this, new GameOverEventArgs(p.Client));
            }
        }

        /// <summary>
        /// Determines whether the specified game contains client.
        /// </summary>
        /// <param name="client">The client.</param>
        /// <returns>
        ///   <c>true</c> if the specified game contains client; otherwise, <c>false</c>.
        /// </returns>
        public bool ContainsClient(IClient client)
        {
            // if found that the given client is already in the players list
            return GetPlayerByClient(client) != null;
        }

        /// <summary>
        /// retrieves a player by its client.
        /// if player is not found, returns null.
        /// </summary>
        /// <param name="client"></param>
        /// <returns></returns>
        private MazeGamePlayer GetPlayerByClient(IClient client)
        {
            return players.Find(x => client.Equals(x.Client));
        }
    }
}