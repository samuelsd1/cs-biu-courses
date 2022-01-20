
/**
 * gets the paramters needed to dra the maze
 * and draw it on given canvas
*/
(function ($) {
    $.fn.drawMaze = function (canvasID, mazeObj, endImage, playerImage) {
        //parse parameters
        var myCanvas = document.getElementById(canvasID);
        var ctx = myCanvas.getContext("2d");

        var rows = mazeObj.Rows;
        var cols = mazeObj.Cols;
        var start = { row: mazeObj.Start.Row, col: mazeObj.Start.Col };
        var end = { row: mazeObj.End.Row, col: mazeObj.End.Col };

        var cellWidth = myCanvas.width / cols;
        var cellHeight = myCanvas.height / rows;

        //console.log("Width: " + cellWidth + ", height: " + cellHeight);
        for (var row = 0; row < rows; row++) {
            for (var col = 0; col < cols; col++) {
                var index = (row * cols) + col;
                //console.log("row: " + row + ", col: " + col + ", index: " + index);
                var x = col * cellWidth;
                var y = row * cellHeight;
                if (mazeObj.Maze[index] == 1) {
                    ctx.fillStyle = '#000000';
                    //console.log("Fillin row,col(" + row + "," + col + ") at (" + x + "," + y + ")");
                } else {
                    ctx.fillStyle = "#F8F8FF";
                }
                
                ctx.fillRect(x, y, cellWidth, cellHeight);

            }
        }
        ctx.drawImage(playerImage, start.col * cellWidth, start.row * cellHeight, cellWidth, cellHeight);
        ctx.drawImage(endImage, end.col * cellWidth, end.row * cellHeight, cellWidth, cellHeight);
    };

    /**
     * redraw the player on canvas according to gives parameters
     */
    $.fn.executeMove = function (canvasName, rows, cols, currPos, newPos, playerImage, ) {
        var myCanvas = document.getElementById(canvasName);
        var context = myCanvas.getContext("2d");

        var cellWidth = myCanvas.width / cols;
        var cellHeight = myCanvas.height / rows;

        // delete the old image
        context.fillStyle = "#F8F8FF";
        context.fillRect(
            cellWidth * currPos.col,
            cellHeight * currPos.row,
            cellWidth, cellHeight);
        // redraw the image
        context.drawImage(playerImage,
            cellWidth * newPos.col,
            cellHeight * newPos.row,
            cellWidth, cellHeight);
        context.fillStyle = "black";
    }
}(jQuery));
