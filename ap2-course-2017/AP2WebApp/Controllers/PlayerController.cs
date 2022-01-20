using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Web.Http;
using WebApp.DataObjects;
using WebApp.Models;


namespace WebApp.Controllers
{
    /// <summary>
    /// the player handling controller
    /// </summary>
    /// <seealso cref="System.Web.Http.ApiController" />
    public class PlayerController : ApiController
    {
        /// <summary>
        /// The model
        /// </summary>
        private static UsersModel model = new UsersModel();

        // GET: api/Player
        /// <summary>
        /// Gets the players.
        /// </summary>
        /// <returns></returns>
        [HttpPost]
        [Route("api/Player/GetPlayers")]
        public IEnumerable<User> GetPlayers()
        {
            return model.GetUsers();
        }

        // GET: api/Player/5
        /// <summary>
        /// Gets the specified identifier.
        /// </summary>
        /// <param name="details">The details.</param>
        /// <returns></returns>
        [HttpPost] //todo why post not working?
        [Route("api/Player/Login")]
        public IHttpActionResult Login(UserLogin details)
        {
            bool exist = model.CheckLogin(details);
            if (exist)
            {
                return Ok<string>("Login details are correct");
            }
            return Unauthorized();
        }

        // POST: api/Player
        /// <summary>
        /// Registers the specified user.
        /// </summary>
        /// <param name="user">The user.</param>
        /// <returns></returns>
        [HttpPost]
        [Route("api/Player/Register")]
        public IHttpActionResult Register(User user)
        {
            bool exist = model.UserExists(user);
            if (exist)
            {
                return Conflict();
            }
            try
            {
                model.RegisterUser(user);
                return Ok<string>("Successfully registered");
            }
            catch (Exception e)
            {
                return InternalServerError(e);
            }
        }

        /// <summary>
        /// Adds the win.
        /// </summary>
        /// <param name="username">The username.</param>
        public static void AddWin(string username)
        {
            model.AddWin(username);
        }

        /// <summary>
        /// Adds the loss.
        /// </summary>
        /// <param name="username">The username.</param>
        public static void AddLoss(string username)
        {
            model.AddLoss(username);
        }
    }
}