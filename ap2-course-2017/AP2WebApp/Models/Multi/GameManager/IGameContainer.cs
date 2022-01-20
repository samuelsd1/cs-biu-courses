using System.Collections.Generic;
using WebApp.Models.Multi.Game;

namespace WebApp.Models.Multi.GameManager
{
    /// <summary>
    /// an interface that defines a game container.
    /// </summary>
    interface IGameContainer
    {
        bool ContainsGame(string name);
        bool ContainsGame(IClient client);
        bool ContainsGame(MazeGame game);

        MazeGame GetGame(string Name);
        MazeGame GetGame(IClient client);

        List<MazeGame> GetNonStartedGames();

        void AddGame(MazeGame game);
        void RemoveGame(MazeGame game);
    }
}
