package main;

/**
 * The object that represents every position on the grid.
 */
public class Position {
    private int row;
    private int col;

    /**
     * Constructor
     * @param row - the row
     * @param col - the col
     */
    public Position(int row, int col){
        this.row = row;
        this.col = col;
    }

    public int getRow(){
        return this.row;
    }

    public int getCol(){
        return this.col;
    }

    // Override all default object functions, because Im using them later, in the algorithm.

    @Override
    public String toString() {
        return row + "," + col;
    }

    @Override
    public int hashCode() {
        return this.toString().hashCode();
    }

    @Override
    public boolean equals(Object obj) {
        if(!(obj instanceof Position)){
            return false;
        }
        Position other = (Position)obj;
        return this.row == other.row && this.col == other.col;
    }
}
