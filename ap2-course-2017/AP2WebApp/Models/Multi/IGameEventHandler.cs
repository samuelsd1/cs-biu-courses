using MazeLib;
using WebApp.Models.Multi.Game;

namespace WebApp.Models.Multi
{
    /// <summary>
    /// a class that handles the event should have thos 3 
    /// functions.
    /// </summary>
    public interface IGameEventHandler
    {
        void HandleGameStarted(MazeGame m);
        void HandleGameOver(MazeGame game, GameOverEventArgs args);
        void HandlePlayerMoved(MazeGame game, IClient player, Direction d);
    }
}
