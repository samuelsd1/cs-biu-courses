using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MazeLib;
using WebApp.Models.Multi.Game;

namespace WebApp.Models.Multi.GameManager
{
    /// <summary>
    /// a class that handles a multiplayer game.
    /// </summary>
    /// <seealso cref="IGameManager" />
    class GameManager : IGameManager
    {
        private IGameContainer container;
        private IGameEventHandler handler;

        /// <summary>
        /// GameManager Ctor
        /// </summary>
        /// <param name="container"></param>
        /// <param name="handler"></param>
        public GameManager(IGameContainer container, IGameEventHandler handler)
        {
            this.container = container;
            this.handler = handler;
        }

        #region handler support

        /// <summary>
        /// Attaches the handlers to the new game.
        /// </summary>
        /// <param name="game">The game.</param>
        private void AttachHandlers(MazeGame game)
        {
            game.onGameOver += this.HandleGameOver;
            game.onGameStarted += handler.HandleGameStarted;
            game.onPlayerMoved += handler.HandlePlayerMoved;
        }

        /// <summary>
        /// Detaches the handlers from the game.
        /// </summary>
        /// <param name="game">The game.</param>
        private void DetachHandlers(MazeGame game)
        {
            game.onGameOver -= this.HandleGameOver;
            game.onGameStarted -= handler.HandleGameStarted;
            game.onPlayerMoved -= handler.HandlePlayerMoved;
        }

        /// <summary>
        /// handles the start of each game
        /// </summary>
        /// <param name="m"></param>
        public void HandleGameStarted(MazeGame m)
        {
            handler.HandleGameStarted(m);
        }

        /// <summary>
        /// handles the end of each game
        /// </summary>
        /// <param name="game"></param>
        /// <param name="args"></param>
        public void HandleGameOver(MazeGame game, GameOverEventArgs args)
        {
            RemoveGame(game);
            handler.HandleGameOver(game, args);
        }

        /// <summary>
        /// handle the player moved event of each game.
        /// </summary>
        /// <param name="game"></param>
        /// <param name="player"></param>
        /// <param name="d"></param>
        public void HandlePlayerMoved(MazeGame game, IClient player, Direction d)
        {
            handler.HandlePlayerMoved(game, player, d);
        }
        
        #endregion

        #region container support
        /// <summary>
        /// Adds the game to the container.
        /// </summary>
        /// <param name="game">The game.</param>
        public void AddGame(MazeGame game)
        {
            if (container.ContainsGame(game))
                throw new InvalidOperationException("Game already exists.");
            AttachHandlers(game);
            container.AddGame(game);
        }

        /// <summary>
        /// removes a game from the container.
        /// </summary>
        /// <param name="game"></param>
        public void RemoveGame(MazeGame game)
        {
            container.RemoveGame(game);
            DetachHandlers(game);
        }

        /// <summary>
        /// Determines whether the specified game with this client is here.
        /// </summary>
        /// <param name="client">The client.</param>
        /// <returns>
        ///   <c>true</c> if the specified client
        ///  contains game; otherwise, <c>false</c>.
        /// </returns>
        public bool ContainsGame(IClient client)
        {
            return container.ContainsGame(client);
        }

        /// <summary>
        /// Determines whether the specified game in this name  is contained here .
        /// </summary>
        /// <param name="Name">The name.</param>
        /// <returns>
        ///   <c>true</c> if the specified name
        ///  contains game; otherwise, <c>false</c>.
        /// </returns>
        public bool ContainsGame(string Name)
        {
            return container.ContainsGame(Name);
        }

        /// <summary>
        /// determines whether the container containes the given game.
        /// </summary>
        /// <param name="game"></param>
        /// <returns></returns>
        public bool ContainsGame(MazeGame game)
        {
            return container.ContainsGame(game);
        }

        /// <summary>
        /// Gets the game by a client.
        /// </summary>
        /// <param name="client">The client.</param>
        /// <returns></returns>
        public MazeGame GetGame(IClient client)
        {
            return container.GetGame(client);
        }

        /// <summary>
        /// Gets the game by game name.
        /// </summary>
        /// <param name="Name">The name.</param>
        /// <returns></returns>
        public MazeGame GetGame(string Name)
        {
            return container.GetGame(Name);
        }

        /// <summary>
        /// return a list of all the non started games.
        /// </summary>
        /// <returns></returns>
        public List<MazeGame> GetNonStartedGames()
        {
            return container.GetNonStartedGames();
        }

        #endregion
    }
}
