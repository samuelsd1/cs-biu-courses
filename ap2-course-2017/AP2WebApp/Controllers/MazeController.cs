using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Web.Http;
using System.Web.Http.Results;
using MazeLib;
using Newtonsoft.Json.Linq;
using SearchAlgorithmsLib;
using WebApp.Models;
using WebApp.DataObjects;


namespace WebApp.Controllers
{
    /// <summary>
    /// the maze handling controller
    /// </summary>
    /// <seealso cref="System.Web.Http.ApiController" />
    public class MazeController : ApiController
    {
        /// <summary>
        /// The model
        /// </summary>
        MazeModel model = new MazeModel();

        // GET: api/Maze
        /// <summary>
        /// Gets this instance.
        /// </summary>
        /// <returns></returns>
        public IEnumerable<string> Get()
        {
            return new string[] {"value1", "value2"};
        }

        // GET: api/Maze/name/algo

        /// <summary>
        /// Solves the specified name.
        /// </summary>
        /// <param name="si">The si.</param>
        /// <returns></returns>
        [HttpPost]
        [Route("api/maze/Solve")]
        public string Solve(SolveInfo si)
        {
            return model.SolveGame(si.Name, si.Algo);
        }

        // GET: api/Maze/s/5/5
        /// <summary>
        /// Generates the specified name.
        /// </summary>
        /// <param name="param">The parameter.</param>
        /// <returns></returns>
        [HttpPost]
        [Route("api/maze/Generate")]
        public IHttpActionResult Generate(GenerateInfo param)
        {
            int row;
            int col;
            row = int.Parse(param.Rows);
            col = int.Parse(param.Cols);
            Maze m = model.GetNewModel(param.Name, row, col);
            if (m == null)
            {
                return BadRequest("Could not generate the requested maze.");
            }
            return Ok(m.ToJSON());
           
        }

        // POST: api/Maze
        /// <summary>
        /// Posts the specified value.
        /// </summary>
        /// <param name="value">The value.</param>
        public void Post([FromBody] string value)
        {
        }


        // PUT: api/Maze/5
        /// <summary>
        /// Puts the specified identifier.
        /// </summary>
        /// <param name="id">The identifier.</param>
        /// <param name="value">The value.</param>
        public void Put(int id, [FromBody] string value)
        {
        }

        // DELETE: api/Maze/5
        /// <summary>
        /// Deletes the specified identifier.
        /// </summary>
        /// <param name="id">The identifier.</param>
        public void Delete(int id)
        {
        }
    }
}