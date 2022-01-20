/**
 * Name:    David Samuelson
 * ID:      208788851
 * Uname:   samueld1
 */
import java.util.ArrayList;
import java.util.List;

/**
 * Node in the minimax algorithm
 */
public class Node {
    private Color p;
    private Board board;

    /**
     * Every node contains board and color of the current player
     * @param b - the board
     * @param p - the current player
     */
    public Node(Board b, Color p) {
        this.board = b;
        this.p = p;
    }

    /**
     * board getter. unused
     * @return
     */
    public Board getBoard(){
        return this.board;
    }

    /**
     * player getter
     * @return
     */
    public Color getPlayer(){
        return p;
    }

    /**
     * generates list of all neighbors
     * @return list of all possible next nodes (the player is the opponent in the next moves)
     */
    public List<Node> neighbors() {
        List<Position> availableMoves = new ArrayList<>();
        for (int row = 0; row < 5; ++row) {
            for (int col = 0; col < 5; ++col) {
                Position curr = new Position(row, col);
                if (board.canPlaceAt(curr)) {
                    availableMoves.add(curr);
                }
            }
        }

        List<Node> neighbors = new ArrayList<>();
        for (Position pos : availableMoves) {
            Board other = new Board(this.board);
            other.placeStone(pos, p);
            neighbors.add(new Node(other, Color.getOpponent(p)));
        }
        return neighbors;
    }

    /**
     * self explanatory
     * @return boolean signifies whether the current node is terminal (has no neighbors)
     */
    public boolean isTerminal() {
        int availableMoves = 0;
        for (int row = 0; row < 5; ++row) {
            for (int col = 0; col < 5; ++col) {
                Position curr = new Position(row, col);
                if (board.canPlaceAt(curr)) {
                    ++availableMoves;
                }
            }
        }

        return (availableMoves == 0);
    }

    /**
     * Heuristic for the current node, as defined
     * @return the heuristic
     */
    public int heuristic() {
        int count = 0;
        int countSides = 0;

        for (int row = 0; row < 5; ++row) {
            for (int col = 0; col < 5; ++col) {
                Color curr = board.getColorAt(row, col);
                // ignore the empty spaces
                if (curr == Color.EMPTY) continue;
                if (curr == p) ++count;
                else --count;

                // count the sides
                if (row == 0 || row == 4 || col == 0 || col == 4) {
                    if (curr == p) ++countSides;
                    else --countSides;
                }
            }
        }

        if (!isTerminal()) {
            return count + countSides;
        }

        if (count > 0)
            return Integer.MAX_VALUE;
        return Integer.MIN_VALUE;
    }

    @Override
    public boolean equals(Object obj) {
        return board.equals(obj);
    }

    @Override
    public int hashCode() {
        return board.hashCode();
    }
}
