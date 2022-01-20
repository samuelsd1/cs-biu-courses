//if you are not logged in then go back to login page
if (sessionStorage.username == undefined) {
    document.location = 'LogIn.html';
}

/**
 * get the default values if entered in setting page
 */
function initVals() {
    $('#cols').val(localStorage.getItem('mazeCols'));
    $('#rows').val(localStorage.getItem('mazeRows'));
}

/**
 * runs when the page is ready and handles
 * the logic
 */
$(document).ready(function () {
    //load the upper nav bar as the page is lodaed
    $("#site-header").load("MenuNav.html");
    initVals();
    //create images
    var myImage = document.getElementById("playerImage");
    var opImage = document.getElementById("opponentImage");
    var endImage = document.getElementById("endImage");
    //create position obects
    var mazeObj;
    var myPos = { row: 0, col: 0 };
    var opPos = { row: 0, col: 0 };

    /*--------------------<Attach SignalR Callbacks>--------------------*/
    var messagesHub = $.connection.MultiPlayerHub;
    messagesHub.client.Hello = function (str) {
        console.log("Hello" + str);
    };
    messagesHub.client.CommandResult = function (commandResJson) {
        handle_command_res(commandResJson);
    };

    //a function that the server ia calling when the game start
    messagesHub.client.GameStarted = function (mazeJson) {
        $("#loading").fadeOut("slow");
        
        sessionStorage.gamestate = 3;
        console.log("Game started ");

        mazeObj = JSON.parse(mazeJson);
        // translate the objects so we will use lower case properties row and col
        var startPos = { row: mazeObj.Start.Row, col: mazeObj.Start.Col };
        myPos.row = startPos.row;
        myPos.col = startPos.col;
        opPos.row = startPos.row;
        opPos.col = startPos.col;

        // draw tha mazes
        $("#myMaze").drawMaze("myMaze", mazeObj, endImage, myImage);
        $("#oppMaze").drawMaze("oppMaze", mazeObj, endImage, opImage);
        alert('Game Started!');
    };

    //a function that the server ia calling when the game is over
    messagesHub.client.GameOver = function (reason) {
        sessionStorage.gamestate = 4;
        alert("Game Over: " + reason);
        document.location = 'rankingPage.html';
    };

    //a function that the server ia calling when the other player moved
    messagesHub.client.PlayerMoved = function (moveUpdateJson) {
        //parse the move
        var mu = JSON.parse(moveUpdateJson);
        var opOldPos = { row: opPos.row, col: opPos.col };
        var moved = true;
        switch (mu.Direction) {
            case "Up":
                opPos.row--;
                break;
            case "Down":
                opPos.row++;
                break;
            case "Left":
                opPos.col--;
                break;
            case "Right":
                opPos.col++;
                break;
            default:
                moved = false;
                break;
        }
        //display it on canvas
        if (moved) {
            $("#myMaze").executeMove("oppMaze",
                mazeObj.Rows, mazeObj.Cols,
                opOldPos, opPos,
                opImage
            );
        }
    };
    /*-------------------</Attach SignalR Callbacks>-------------------*/

    /*-----------------------<Connect To The Hub>-----------------------*/
    console.log("Connecting to SignalR Hub");
    $.connection.hub.start()
        .done(function () {
            connect();
            // start after 500ms, let it connect to be sure all is working good.
            setTimeout(start(), 500);
        })
        .fail(function () {
            console.log("failed to connect");
        });
    /*----------------------</Connect To The Hub>----------------------*/

    /**
     *connects to the hub
     */
    function connect() {
        // make sure no problems with session
        if (sessionStorage.username == undefined) {
            document.location = 'LogIn.html';
        }
        messagesHub.server.subscribe(sessionStorage.username);
    }

    /**
     * start the game - send request to server
     */
    function start() {
        /*---------------------<Attach Event Handlers>---------------------*/
        $(window).bind('beforeunload', function () {
            if (sessionStorage.gamestate) {
                var state = parseInt(sessionStorage.gamestate);
                if (state == 2 || state == 3) {
                    send_command("close xd");
                }
            }
        });
        //when you click the refreshe button - ask from hub to the games
        $("#refresh").on('click', function (e) {
            e.preventDefault();
            send_command("list");
        });
        //when you click the start button send request to hub
        $("#startButton").on('click', function (e) {
            e.preventDefault();

            var gameName = $("#gameName").val();
            var rows = $("#rows").val();
            var cols = $("#cols").val();

            send_command("start " + gameName + " " + rows + " " + cols);
        });

        //when you click the join button send request to hub
        $("#joinButton").on('click', function (e) {
            e.preventDefault();
            console.log(document.getElementById('gameList').selectedIndex);

            if (document.getElementById('gameList').selectedIndex < 0) {
                alert('Please select a game...');
                return;
            }
            var gameName = $('#gameList').find(":selected").text();
            send_command("join " + gameName);
        });

        // attach keyup event handler
        $("body").on("keyup", function (arg) {
            handle_key_up(arg.which);
        });
        /*---------------------</Attach Event Handlers>---------------------*/

        sessionStorage.gamestate = 1;
        send_command("list");
    }


    /*-------------------------<Event Handlers>-------------------------*/
    //handles the answers from server
    function handle_command_res(cmdResJson) {
        var res = JSON.parse(cmdResJson);
        if (!res.Success) {
            console.log("Command " + res.Command + " has failed. Reason: " + res.Data);
            alert(res.Data);
            return;
        }
        //check which result we got back
        switch (res.Command) {
            case "List":
                gameList = JSON.parse(res.Data);

                clear_select();
                for (var i = 0; i < gameList.length; ++i) {
                    add_option_to_select(gameList[i]);
                }
                break;
            case "Start":
                sessionStorage.gamestate = 2;
                alert(res.Data);
                $("#loading").fadeIn("slow");
                break;
            case "Join":
                if (sessionStorage.gamestate) {
                    var state = parseInt(sessionStorage.gamestate);
                    // if game not started yet
                    if (state < 2) {
                        sessionStorage.gamestate = 2;
                        alert('Joined the game. Please wait for your opponent to connect');
                        $("#loading").fadeIn("slow");
                    }
                }
                break;
            case "Play":
                handle_my_movement(res.Data);
                break;
        }
    }
    /**
     * if i moved then display it
     * and send memo to the server
     */
    function handle_my_movement(direction) {
        var myOldPos = { row: myPos.row, col: myPos.col };
        var moved = true;
        switch (direction) {
            case "Up":
                myPos.row--;
                break;
            case "Down":
                myPos.row++;
                break;
            case "Left":
                myPos.col--;
                break;
            case "Right":
                myPos.col++;
                break;
            default:
                moved = false;
                break;
        }
        //display my move
        if (moved) {
            $("#myMaze").executeMove("myMaze",
                mazeObj.Rows, mazeObj.Cols,
                myOldPos, myPos,
                myImage
            );
        }
    }
    /**
     * the event that listen to my movements
     */
    function handle_key_up(key) {
        var oldRow = myPos.row;
        var oldCol = myPos.col;
        switch (key) {
            //key left
            case 37:
                send_command("play left");
                break;
            //key up
            case 38:
                send_command("play up");
                break;
            //key right
            case 39:
                send_command("play right");
                break;
            //key down
            case 40:
                send_command("play down");
                break;
            default:
                break;
        }
    }
    /*------------------------</Event Handlers>------------------------*/

    /*-------------------<Additional Functionalilty>-------------------*/
    function send_command(cmd) {
        console.log("Sending command: \"" + cmd + "\"");
        messagesHub.server.execute(cmd);
    }
    /**
     * clear the join game list
     */
    function clear_select() {
        // clear the list
        $("#gameList")
            .find('option')
            .remove()
            .end()
            .append('<option value="" disabled selected>Select your option</option>')
            .val("");
    }

    /**
     * add option the join game list
     */
    function add_option_to_select(opt) {
        $("#gameList")
            .append('<option value="' + opt + '">' + opt + '</option>')
            .val(opt);
    }
    /*-------------------</Additional Functionalilty>-------------------*/
});
