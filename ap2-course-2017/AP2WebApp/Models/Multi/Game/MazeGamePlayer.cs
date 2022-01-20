using MazeLib;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WebApp.Models.Multi.Game
{
    /// <summary>
    /// a class that extend the abstract player.
    /// and in charge of a maze player logic.
    /// </summary>
    /// <seealso cref="Model.Game.Player" />
    class MazeGamePlayer
    {
        public IClient Client { get; }
        public Position Position { get; set; }

        /// <summary>
        /// Initializes a new instance of the <see cref="MazeGamePlayer"/> class.
        /// </summary>
        /// <param name="client">The client.</param>
        /// <param name="initialPos">The initial position.</param>
        public MazeGamePlayer(IClient client, Position initialPos)
        {
            this.Client = client;
            this.Position = initialPos;
        }

        /// <summary>
        /// Gets the estimated position.
        /// </summary>
        /// <param name="d">The d.</param>
        /// <returns></returns>
        public Position GetEstimatedPos(Direction d)
        {
            int row = Position.Row;
            int col = Position.Col;
            switch (d)
            {
                case Direction.Up:
                    --row;
                    break;
                case Direction.Down:
                    ++row;
                    break;
                case Direction.Left:
                    --col;
                    break;
                case Direction.Right:
                    ++col;
                    break;
            }
            return new Position(row, col);
        }

        #region override object functions

        /// <summary>
        /// Determines whether the specified <see cref="System.Object" />,
        ///  is equal to this instance.
        /// </summary>
        /// <param name="obj">The <see cref="System.Object" /> 
        /// to compare with this instance.</param>
        /// <returns>
        ///   <c>true</c> if the specified <see cref="System.Object" />
        ///  is equal to this instance; otherwise, <c>false</c>.
        /// </returns>
        public override bool Equals(object obj)
        {
            return Equals(obj as MazeGamePlayer);
        }

        /// <summary>
        /// Equalses the specified other.
        /// </summary>
        /// <param name="other">The other.</param>
        /// <returns></returns>
        public bool Equals(MazeGamePlayer other)
        {
            if (other == null)
                return false;
            return this.Client.Equals(other.Client);
        }

        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        /// <returns>
        /// A hash code for this instance, 
        /// suitable for use in hashing algorithms and data structures like a hash table. 
        /// </returns>
        public override int GetHashCode()
        {
            return this.Client.GetHashCode();
        }

        #endregion
    }
}
