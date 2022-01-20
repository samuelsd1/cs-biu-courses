
namespace WebApp.Models.Multi.GameManager
{
    /// <summary>
    /// an interface that define a game manager.
    /// </summary>
    /// <seealso cref="IGameContainer" />
    /// <seealso cref="IGameEventHandler" />
    interface IGameManager : IGameContainer, IGameEventHandler
    {
    }
}
