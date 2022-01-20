/**
 * Name:    David Samuelson
 * ID:      208788851
 * Uname:   samueld1
 */
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;

/**
 * main class
 */
public class java_ex2 {

    /**
     * The minimax function
     * @param root - the root node
     * @param depth - remaining depth to go
     * @param maximizing - is the current player maximizing
     * @return the best next node for the maximizing player.
     */
    public static Node minmax(Node root, int depth, boolean maximizing) {
        if (root.isTerminal() || depth == 0) {
            return root;
        }

        List<Node> neighbors = root.neighbors();
        Node bestNode = neighbors.get(0);
        for (Node child : neighbors) {
            Node next = minmax(child, depth - 1, !maximizing);

            if (maximizing) {
                if (next.heuristic() > bestNode.heuristic()) {
                    bestNode = child;
                }
            } else {
                if (next.heuristic() < bestNode.heuristic()) {
                    bestNode = child;
                }
            }
        }
        return bestNode;
    }

    /**
     * main
     * @param args - unused
     */
    public static void main(String[] args) {
        try {
            Board b = InputParser.parseInput("input.txt");
            Node root = new Node(b, Color.BLACK);

            boolean max = true;
            while (!root.isTerminal()) {
                root = minmax(root, 3, max);
                max = !max;
            }

            Color winning = (root.heuristic() < 0) ? Color.getOpponent(root.getPlayer()) : root.getPlayer();
            System.out.println("WinningPlayer: " + winning);
            String win = (winning == Color.BLACK) ? "B" : "W";
            Files.write(Paths.get("output.txt"), win.getBytes());
        } catch (Exception e) {
            System.out.println("Exception: " + e.getMessage());
            System.out.println(e.getMessage());
        }
    }
}
