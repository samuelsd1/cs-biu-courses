using MazeLib;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace WebApp.Controllers.Multi
{
    /// <summary>
    /// a class taht in charge of getting a string
    /// and return a matching direction
    /// </summary>
    public class DirectionParser
    {
        /// <summary>
        /// Tries the parse.and initialze the new direction.
        /// </summary>
        /// <param name="dirStr">The dir string.</param>
        /// <param name="dir">The dir.</param>
        /// <returns></returns>
        public static bool TryParse(string dirStr, out Direction dir)
        {
            switch (dirStr)
            {
                case "up":
                    dir = Direction.Up;
                    break;
                case "down":
                    dir = Direction.Down;
                    break;
                case "left":
                    dir = Direction.Left;
                    break;
                case "right":
                    dir = Direction.Right;
                    break;
                default:
                    dir = Direction.Unknown;
                    return false;
            }
            return true;
        }
    }
}