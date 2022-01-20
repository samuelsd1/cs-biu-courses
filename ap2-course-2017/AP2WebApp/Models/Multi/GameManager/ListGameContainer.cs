using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using WebApp.Models.Multi.Game;

namespace WebApp.Models.Multi.GameManager
{
    /// <summary>
    /// a class that implement the container interface.
    /// </summary>
    class ListGameContainer : IGameContainer
    {
        private List<MazeGame> games;

        /// <summary>
        /// Initializes a new instance of ListGameContainer
        /// </summary>
        public ListGameContainer()
        {
            games = new List<MazeGame>();
        }

        /// <summary>
        /// Adds the game.
        /// </summary>
        /// <param name="game">The game.</param>
        public void AddGame(MazeGame game)
        {
            games.Add(game);
        }

        /// <summary>
        /// Removes the game.
        /// </summary>
        /// <param name="game">The game.</param>
        public void RemoveGame(MazeGame game)
        {
            games.Remove(game);
        }

        /// <summary>
        /// returns whether the container containes a game with the given client.
        /// </summary>
        /// <param name="client">The client.</param>
        /// <returns>
        ///   <c>true</c> if the game with this client 
        /// contained ; otherwise, <c>false</c>.
        /// </returns>
        public bool ContainsGame(IClient client)
        {
            return GetGame(client) != null;
        }

        /// <summary>
        /// Determines whether there is a game with the given name.
        /// </summary>
        /// <param name="Name">The name.</param>
        /// <returns>
        ///   <c>true</c> if the specified 
        /// name contains game; otherwise, <c>false</c>.
        /// </returns>
        public bool ContainsGame(string Name)
        {
            return GetGame(Name) != null;
        }

        /// <summary>
        /// determines whether the container containes the given game.
        /// </summary>
        /// <param name="game"></param>
        /// <returns></returns>
        public bool ContainsGame(MazeGame game)
        {
            return games.Contains(game);
        }

        /// <summary>
        /// Gets the game.
        /// </summary>
        /// <param name="client">The client.</param>
        /// <returns></returns>
        public MazeGame GetGame(IClient client)
        {
            return games.Find(x => x.ContainsClient(client));
        }

        /// <summary>
        /// Gets the game.
        /// </summary>
        /// <param name="Name">The name.</param>
        /// <returns></returns>
        public MazeGame GetGame(string Name)
        {
            return games.Find(x => Name.Equals(x.Name));
        }

        /// <summary>
        /// Gets the non started games.
        /// </summary>
        /// <returns></returns>
        public List<MazeGame> GetNonStartedGames()
        {
            return games.Where(x => !x.Started).ToList();
        }
    }
}
