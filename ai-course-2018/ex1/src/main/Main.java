package main;

import main.graph.AlgorithmType;
import main.graph.GraphSearcher;
import main.graph.Heuristic;
import main.graph.Solution;
import main.graph.searchers.AStarSearcher;
import main.graph.searchers.IDSSearcher;

import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        if (args.length < 1) {
            System.out.println("Usage: <input_filename>");
            return;
        }

        int n;
        AlgorithmType type;
        GridGraph gridGraph;
        try {
            // parse, get the map size, get algorithm type, and create the what so called "graph"
            ParseResult p = GridParser.ParseGrid(args[0]);
            n = p.getGrid().length;
            type = p.getType();
            gridGraph = new GridGraph(p.getGrid());

        } catch (Exception e) {
            // if something happened, i just print and quit.
            System.out.println(e.getMessage());
            System.out.println("Exiting...");
            return;
        }

        // heuristic using lambda, because its awesome
        Heuristic<Position> airDistanceHeuristic = (s, t) -> {
            int d1 = s.getRow() - t.getRow();
            int d2 = s.getCol() - t.getCol();
            return Math.sqrt(d1 * d1 + d2 * d2);
        };

        // well, this is self explanatory
        GraphSearcher<Position> searcher;
        switch (type) {
            case AStar:
                searcher = new AStarSearcher<>(airDistanceHeuristic);
                break;
            case IDS:
                searcher = new IDSSearcher<>(n * n);
                break;
            default:
                throw new RuntimeException("No Algorithm Defined");
        }

        // get the solution, then extract the path
        Solution<Position> sol = searcher.search(gridGraph);
        List<Position> path = gridGraph.computePath(sol);

        // create the output, using string builder (actually the IDE recommended me to do that)
        StringBuilder sb = new StringBuilder();

        // if no path found
        if (path.size() <= 0) {
            // this is the desired output
            sb.append("no path");
        } else {
            // else, we did find a path. get the output representation of the path, and compute the cost.
            List<String> outputRepr = deltaRepresentation(path);
            double cost = gridGraph.getTotalCost(path);

            // create the desired output string, in a lazy way
            for (String bt : outputRepr) {
                sb.append(bt);
                sb.append("->");
            }
            // delete last arrow, because im lazy.
            sb.delete(sb.length() - 2, sb.length());
            // add the cost
            sb.append(" ");
            sb.append((int) cost);
        }

        // now - write the desired output to the output file.
        try {
            Files.write(Paths.get("output.txt"), sb.toString().getBytes());
        } catch (Exception e) {
            System.out.println("Could not write result to output file.");
        }
        // im documenting too much, right?
    }

    /**
     * returns a string that describes the move, when the move between two
     * positions is represented using the delta in the rows and cols (target-src)
     * @param drow - delta in the row axis
     * @param dcol - delta in the col axis
     * @return string which represents the move.
     */
    private static String strByDelta(int drow, int dcol) {
        if (drow == 0 && dcol == 1) {
            return "R";
        } else if (drow == 1 && dcol == 1) {
            return "RD";
        } else if (drow == 1 && dcol == 0) {
            return "D";
        } else if (drow == 1 && dcol == -1) {
            return "LD";
        } else if (drow == 0 && dcol == -1) {
            return "L";
        } else if (drow == -1 && dcol == -1) {
            return "LU";
        } else if (drow == -1 && dcol == 0) {
            return "U";
        } else if (drow == -1 && dcol == 1) {
            return "RU";
        } else {
            // just for safety and code without warnings.
            throw new RuntimeException("Unknown delta " + drow + "," + dcol);
        }
    }

    /**
     * Returns a list which represents the path using the delta representation.
     * @param path - list of positions which construct the path from start to goal
     * @return - list which represents the path as defined.
     */
    private static List<String> deltaRepresentation(List<Position> path) {
        List<String> backtrace = new ArrayList<>();
        for (int i = 1; i < path.size(); ++i) {
            Position curr = path.get(i);
            Position prev = path.get(i - 1);
            int drow = curr.getRow() - prev.getRow();
            int dcol = curr.getCol() - prev.getCol();
            String str = strByDelta(drow, dcol);
            backtrace.add(str);
        }
        return backtrace;
    }
}