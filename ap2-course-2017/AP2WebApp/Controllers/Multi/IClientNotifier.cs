using SharedData;
using WebApp.Models.Multi;

namespace WebApp.Controllers.Multi
{
    /// <summary>
    /// an interface that define what 
    /// someone that want to be in touch with client
    /// should have.
    /// </summary>
    public interface IClientNotifier
    {
        void NotifyClient(IClient client, Notification notif);
    }
}
