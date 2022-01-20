/**
 * Name:    David Samuelson
 * ID:      208788851
 * Uname:   samueld1
 */
/**
 * The board, which contains all the reversi logic.
 */
public class Board {
    private Color[][] board;

    /**
     * default constructor
     */
    public Board(){
         this.board = new Color[5][5];
         for(int i=0;i<5;++i){
             for(int j=0;j<5;++j){
                 this.board[i][j] = Color.EMPTY;
             }
         }
    }

    /**
     * Copy constructor
     * @param existing - an existing board
     */
    public Board(Board existing){
        this.board = new Color[5][5];
        for(int i=0;i<5;++i){
            for(int j=0;j<5;++j){
                this.board[i][j] = existing.board[i][j];
            }
        }
    }

    /**
     *
     * SOME GETTERS AND SETTERS
     *
     */

    public void setCell(int row, int col, Color p){
        board[row][col] = p;
    }
    public Color getColorAt(int row, int col){
        return this.board[row][col];
    }
    public Color getColorAt(Position p){
        return getColorAt(p.row,p.col);
    }


    /**
     * @param row - row
     * @param col - column
     * @return true if the given position is inside the board
     */
    public boolean isInBoard(int row, int col){
        return (row >= 0 && row < 5) && (col >= 0 && col < 5);
    }

    public boolean isInBoard(Position p){
        return isInBoard(p.row, p.col);
    }

    /**
     * Counts how many stones would be flipped in the given direction [dx,dy]
     * if we place stone in the position 'curr', when the color of the
     * current player is p.
     * @param curr - position of the placed stone
     * @param p - the players color
     * @param dy - delta y (used to represent direction)
     * @param dx - delta x (used to represent direction)
     * @return the number of the stones that would be flipped.
     */
    private int countFlip(Position curr, Color p, int dy, int dx){
        int row, col;
        int counter = 0;

        if(dx == 0 && dy == 0){
            return 0;
        }

        row = curr.row + dy;
        col = curr.col + dx;

        for(; isInBoard(row,col); row+=dy, col += dx){
            if(this.board[row][col] == Color.EMPTY){
                counter = 0;
                break;
            }

            if(this.board[row][col] == p){
                break;
            }

            ++counter;
        }

        // if the edge of the board is reached, clear the counter
        if(!isInBoard(row,col)){
            counter = 0;
        }

        // if the counter is not zero, the move is legal and the counter holds how pieces will flip in this direction
        return counter;
    }

    /**
     * Function returns if a stone with the color p can be placed
     * at the position 'curr'.
     * @param curr - the position to place the stone
     * @return 1 if a stone can be placed at the position 'curr'
     */
    public boolean canPlaceAt(Position curr){
        int flipped;
        int dx,dy;

        // To see if it is legal to place a piece of color p at position x,y, do:
        // If the x,y square is already occupied, the move is not legal
        if (this.board[curr.row][curr.col] != Color.EMPTY) {
            return false;
        }

        flipped = 0;
        // for each direction
        for (dy = -1; dy <= 1; ++dy) {
            for (dx = -1; dx <= 1; ++dx) {
                if(dx == 0 && dy == 0){
                    continue;
                }
                int row,col;
                row = curr.row + dy;
                col = curr.col + dx;
                if(!isInBoard(row,col)) continue;
                if(this.board[row][col] != Color.EMPTY){
                    return true;
                }
            }
        }
        // If we've checked all the directions and haven't flipped any pieces, the move is not legal
        return false;
    }

    /**
     * flips the stones after stone with color p was placed at the position 'curr'
     * towards the direction [dx,dy].
     * @param curr - position of the placed stone
     * @param p - color of the placed stone
     * @param dy - delta y, used to represent direction
     * @param dx - delta x, used to represent direction
     */
    private int flip(Position curr, Color p, int dy, int dx){
        int row, col;
        int counter = 0;
        // except no direction
        if (dx == 0 && dy == 0)
            return 0;

        // if current position is no in the color p
        if (this.board[curr.row][curr.col] != p)
            return 0;

        // if there are no flipped stones in that direction, dont flip.
        if (countFlip(curr, p, dy, dx) <= 0) {
            return 0;
        }

        // Scan outward from the x,y position
        row = curr.row + dy;
        col = curr.col + dx;
        for (; isInBoard(row, col); row += dy, col += dx) {
            // If a blank square or the edge of the board is reached, clear the counter and stop scanning
            if (this.board[row][col] == Color.EMPTY) {
                break;
            }
            // If a piece of player color is found, stop scanning
            if (this.board[row][col] == p) {
                break;
            }

            // the color is opposite. flip it.
            this.board[row][col] = p;
            ++counter;
        }
        return counter;
    }
    /**
     * places a stone in the given position (if it is legal position)
     * @param curr - position to place the stone
     * @param p - color of the stone
     */
    public void placeStone(Position curr, Color p) {
        int dx, dy;
        if (!canPlaceAt(curr)) {
            throw new RuntimeException("Cant place stone at " + curr.row + ", " + curr.col);
        }

        // place the stone
        this.board[curr.row][curr.col] = p;

        // for each direction
        for (dy = -1; dy <= 1; ++dy) {
            for (dx = -1; dx <= 1; ++dx) {
                int flipped = countFlip(curr, p, dy, dx);
                if(flipped > 0){
                    flip(curr, p, dy, dx);
                }
            }
        }
    }

    @Override
    public boolean equals(Object obj) {
        if(!(obj instanceof  Board))
            return false;
        Board other = (Board)obj;
        for(int i=0;i<5;++i){
            for(int j=0;j<5;++j){
                if(this.board[i][j]!= other.board[i][j])
                    return false;
            }
        }
        return true;
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        for(int row=0;row<5;++row){
            sb.append(" ___ ___ ___ ___ ___\n");
            sb.append("|");
            for(int col=0;col<5;++col){
                Color curr = board[row][col];
                String ch;
                switch (curr){
                    case BLACK:
                        ch = "B";
                        break;
                    case WHITE:
                        ch = "W";
                        break;
                    default:
                        ch = " ";
                        break;

                }
                sb.append(" " + ch + " |");
            }
            sb.append("\n");
        }
        sb.append(" ___ ___ ___ ___ ___\n");
        return sb.toString();
    }
}
