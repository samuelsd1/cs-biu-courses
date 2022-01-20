
/**
 * the function is up when the page is ready and create
 * a nave bar,display user name if logged in.
 */
$(document).ready(function () {
    if (sessionStorage.username !== undefined) {
        // append username
        $("#logIn").html("<li><a>Hello, " + sessionStorage.username + " !</a></li>");
        $("#logIn").append("<li><a href='javascript:logout();'> Log Out</a></li>");
    } else {
        // append login buttons
        $("#logIn").html('<li><a href="LogIn.html">Login</a></li>');
        $("#logIn").append('<li><a href="Register.html">Register</a></li>');
    }
});

/**
 * stop display user name
 */
function logout() {
    if (sessionStorage.username) {
        sessionStorage.removeItem('username');
        document.location = 'HomePage.html';
    }
}