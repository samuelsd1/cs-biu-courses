
/**
 * get the default values if entered in setting page
 */
function initVals() {
    $('#Cols').val(localStorage.getItem('mazeCols'));
    $('#Rows').val(localStorage.getItem('mazeRows'));
    $('#algos').val((localStorage.getItem('searchAlgo')));
}

/**
 * the main function
 * handle all events and logic.
 */
function func() {
    initVals();

    // game state:
    // 0 - not started
    // 1 - received maze, playin
    // 2 - solving maze
    // 3 - won
    var gameState = 0;
    var mazeObj;
    var submited = false;

    var pos = { row: 0, col: 0 };

    var img = document.getElementById("playerImage");
    var endImage = document.getElementById("endImage");

    //handle the solve request
    $("#solveButton").on("click",
        function () {
            if (gameState != 1) {
                return;
            }
            var e = document.getElementById("algos");
            if (e.selectedIndex < 0) {
                alert("choose algo");
                return;
            }

            var algo = e.options[e.selectedIndex].text;
            var data = {
                Name: name,
                Algo: algo
            };
            //makke the ajax request
            $.ajax({
                type: "Post",
                url: "api/Maze/Solve/",
                contentType: "application/json; charset=utf-8",
                data: JSON.stringify(data),
                dataType: "json",
                success: function (data) {
                    var i = 0;
                    if (data == "not such maze") {
                        alert("A maze with that name already exists.");
                        return;
                    }

                    gameState = 2;

                    var oldPos = { row: pos.row, col: pos.col };
                    pos.row = mazeObj.Start.Row;
                    pos.col = mazeObj.Start.Col;

                    $("#mazeCanvas").executeMove("mazeCanvas",
                        mazeObj.Rows, mazeObj.Cols,
                        oldPos, pos,
                        img
                    );
                    //run in intervals
                    var timer = setInterval(function () {
                        oldPos = { row: pos.row, col: pos.col };
                        switch (data[i]) {
                            case '0': //go left
                                pos.col--;
                                break;
                            case '1': //go right
                                pos.col++;
                                break;
                            case '2': //go up
                                pos.row--;
                                break;
                            case '3': //go down
                                pos.row++;
                                break;
                            default:
                                // server error. will not happen in this exercise.
                                break;
                        }
                        //draw curr move
                        $("#mazeCanvas").executeMove("mazeCanvas",
                            mazeObj.Rows, mazeObj.Cols,
                            oldPos, pos,
                            img
                        );
                        i++;
                        //if finish display message
                        if (won()) {
                            clearInterval(timer);
                            gameState = 3;
                            alert("you win");
                            document.location = 'HomePage.html';
                        }
                    }, 400);
                },
                error: function (xhr, textStatus, errorThrown) {
                    $("#results").html("<li>error</li>");
                }
            });

        });
    //register the the submit event
    $("#requestForm").submit(function (e) {
        e.preventDefault();
        if (!submited) {
            //get the values
            rows = $("#Rows").val();
            cols = $("#Cols").val();
            name = $("#name").val();

            if ((name == "") || (rows == "") || (cols == "")) {
                alert("please enter all fields");
            } else {
                submited = true;
                var url = "api/Maze/Generate";
                var data = {
                    Name: name,
                    Rows: rows,
                    Cols: cols
                };
                //make the ajax request
                $.ajax({
                    type: "Post",
                    url: url,
                    contentType: "application/json; charset=utf-8",
                    data: JSON.stringify(data),
                    dataType: "json",
                    success: function (response) {
                        gameState = 1;
                        mazeObj = JSON.parse(response);
                        $("#mazeCanvas").drawMaze("mazeCanvas", mazeObj, endImage, img);

                        pos.row = mazeObj.Start.Row;
                        pos.col = mazeObj.Start.Col;
                    },
                    error: function (xhr, textStatus, errorThrown) {
                        submited = false;
                        alert("a maze in this name already exist");
                    }
                });
            }
        }
    });
    //register to the click on restart button
    $("#restartButton").on("click", function (e) {
        e.preventDefault();
        if (gameState == 1) {
            pos = { row: mazeObj.Start.Row, col: mazeObj.Start.Col };
            $("#mazeCanvas").drawMaze("mazeCanvas", mazeObj, endImage, img);
        }
    });

    /**
     *check if request movemnet is leagal
     */
    function canMoveTo(row, col) {

        if (!mazeObj) {
            return false;
        }
        //check the movement
        var rows = mazeObj.Rows;
        var cols = mazeObj.Cols;

        if (row < 0 || row >= rows)
            return false;

        if (col < 0 || col >= cols)
            return false;

        var index = (row * cols) + col;
        var can = (mazeObj.Maze[index] == '0');
        return can;
    }
    /**
     * handle my winning
     */
    function won() {
        if (!mazeObj)
            return false;
        return ((pos.col == mazeObj.End.Col) && (pos.row == mazeObj.End.Row));
    }

    //register and listen to player moves
    $("body").on("keyup",
        function (arg) {

            // if game state is not at "currently playing" (= 1), then
            // dont handle this event.
            if (gameState != 1)
                return;

            var moved = false;
            var oldPos = { row: pos.row, col: pos.col };

            switch (arg.which) {
                //key left
                case 37:
                    if (canMoveTo(pos.row, pos.col - 1)) {
                        pos.col--;
                        moved = true;
                    }
                    break;
                //key up
                case 38:
                    ; if (canMoveTo(pos.row - 1, pos.col)) {
                        pos.row--;
                        moved = true;
                    }
                    break;
                //key right
                case 39:
                    if (canMoveTo(pos.row, pos.col + 1)) {
                        pos.col++;
                        moved = true;
                    }
                    break;
                //key down
                case 40:
                    if (canMoveTo(pos.row + 1, pos.col)) {
                        pos.row++;
                        moved = true;
                    }
                    break;
                default:
                    break;
            }
            //drew my movement
            if (moved) {
                $("#myMaze").executeMove("mazeCanvas",
                    mazeObj.Rows, mazeObj.Cols,
                    oldPos, pos,
                    img
                );
                //check if i won
                if (won()) {
                    alert("you win");
                    document.location = 'HomePage.html';
                }
            }

        });
}

/**
 * load the upper nav bar as the page is lodaed
 */
$(document).ready(function () {
    $("#site-header").load("MenuNav.html");
    func();
});