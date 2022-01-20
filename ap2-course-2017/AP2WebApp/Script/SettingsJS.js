/**
 * load the upper nav bar as the page is lodaed
 */
$(document).ready(function () {
    $("#site-header").load("MenuNav.html");
});

/**
 * validate the input
 * then store it.
 */
function Store() {
    var inputRows = $('#rows').val();
    var inputCol = $('#cols').val();

    var e = document.getElementById("algos");
    var inputAlgo = e.options[e.selectedIndex].text;
    var row;
    var col;
    //validate the input
    if ((inputRows == "") || (inputCol == "")) {
        alert("fields must be filled out");
    } else {
        col = parseInt(inputCol, 10);
        row = parseInt(inputRows, 10);
        if ((isNaN(col)) || (isNaN(row))) {
            alert("enter a number not a string");
            //save it
        } else {
            localStorage.setItem('mazeRows', row);
            localStorage.setItem('mazeCols', col);
            localStorage.setItem('searchAlgo', inputAlgo);
            document.location = "HomePage.html";
        }
    }
}
