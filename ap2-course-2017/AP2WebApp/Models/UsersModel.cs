using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using WebApp.DataObjects;
using System.Data.OleDb;
using System.Data;
using System.Data.SqlClient;

namespace WebApp.Models
{
    /// <summary>
    /// handles all the user action
    /// </summary>
    public class UsersModel
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="UsersModel"/> class.
        /// </summary>
        public UsersModel()
        {

        }

        /// <summary>
        /// Gets the users.
        /// </summary>
        /// <returns></returns>
        public List<User> GetUsers()
        {
            string query = "SELECT userName,email,wins,loss FROM Users";
            DataTable table = ExecuteQuery(query);

            List<User> users = new List<User>();
            foreach(DataRow row in table.Rows)
            {
                User u = new User();
                u.Name = row["userName"] as string;
                u.Password = "TupacIsAlive";
                u.Email = row["email"] as string;
                u.Wins = (row["wins"] as int?).ToString();
                u.Losses = (row["loss"] as int?).ToString();
                users.Add(u);
            }
            return users;
        }

        /// <summary>
        /// Gets the user.
        /// </summary>
        /// <param name="username">The username.</param>
        /// <returns></returns>
        public DataRow GetUser(string username)
        {
            DataTable users = ExecuteQuery(string.Format("SELECT * FROM Users WHERE userName = '{0}'", username));
            if(users.Rows.Count > 0)
            {
                return users.Rows[0];
            }
            return null;
        }

        /// <summary>
        /// Users the exists.
        /// </summary>
        /// <param name="username">The username.</param>
        /// <returns></returns>
        public bool UserExists(string username)
        {
            return GetUser(username) != null;
        }

        /// <summary>
        /// Checks the register.
        /// </summary>
        /// <param name="user">The check.</param>
        /// <returns></returns>
        public bool UserExists(User user)
        {
            return UserExists(user.Name);
        }

        /// <summary>
        /// Registers the user.
        /// </summary>
        /// <param name="user">The user.</param>
        /// <returns></returns>
        public bool RegisterUser(User user)
        {
            if (UserExists(user))
                return false;

            string query = string.Format("INSERT INTO Users VALUES ('{0}','{1}', '{2}', {3}, {4})",
                user.Name,
                user.Password,
                user.Email,
                0,
                0);
            ExecuteNonQuery(query);
            return true;
        }


        /// <summary>
        /// Checks the login.
        /// </summary>
        /// <param name="det">The det.</param>
        /// <returns></returns>
        public bool CheckLogin(UserLogin det)
        {
            string query = string.Format("SELECT userName FROM Users WHERE userName = '{0}' AND password = '{1}'",
                det.UserName, det.Password);
            DataTable table = ExecuteQuery(query);

            return (table.Rows.Count > 0);
        }

        /// <summary>
        /// Adds the win.
        /// </summary>
        /// <param name="username">The username.</param>
        /// <returns></returns>
        public bool AddWin(string username)
        {
            DataRow user = GetUser(username);
            if (user == null)
                return false;

            int wins = 0;
            int? winsObj = user["wins"] as int?;
            if(winsObj.HasValue)
            {
                wins = winsObj.Value;
            }
            ++wins;
            ExecuteNonQuery(string.Format("UPDATE Users SET wins = {0} WHERE userName = '{1}'", wins, username));
            return true;
        }

        /// <summary>
        /// Adds the loss.
        /// </summary>
        /// <param name="username">The username.</param>
        /// <returns></returns>
        public bool AddLoss(string username)
        {
            DataRow user = GetUser(username);
            if (user == null)
                return false;

            int loss = 0;
            int? lossObj = user["loss"] as int?;
            if (lossObj.HasValue)
            {
                loss = lossObj.Value;
            }
            ++loss;
            ExecuteNonQuery(string.Format("UPDATE Users SET loss = {0} WHERE userName = '{1}'", loss, username));
            return true;
        }

        /// <summary>
        /// Executes the query.
        /// </summary>
        /// <param name="query">The query.</param>
        /// <returns></returns>
        private static DataTable ExecuteQuery(string query)
        {
            string connStr = @"Data Source=(LocalDB)\MSSQLLocalDB;AttachDbFilename=|DataDirectory|\UsersDB.mdf; Integrated Security = True";
            DataTable ds = new DataTable();
            try
            {
                using (SqlConnection cn = new SqlConnection(connStr))
                using (SqlCommand cmd = new SqlCommand(query, cn))
                using (SqlDataAdapter adapter = new SqlDataAdapter(cmd))
                {
                    cn.Open();
                    adapter.Fill(ds);
                }
            }catch(Exception e)
            {
                // do nothingn...
                
            }
            return ds;
        }

        /// <summary>
        /// Executes the non query.
        /// </summary>
        /// <param name="query">The query.</param>
        private static void ExecuteNonQuery(string query)
        {
            string connStr = @"Data Source=(LocalDB)\MSSQLLocalDB;AttachDbFilename=|DataDirectory|\UsersDB.mdf; Integrated Security = True";

            using (SqlConnection conn = new SqlConnection(connStr))
            using (SqlCommand cmd = new SqlCommand(query, conn))
            {
                conn.Open();
                cmd.ExecuteNonQuery();
            }
        }
    }
}