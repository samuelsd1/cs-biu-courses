package main.graph.searchers;

import main.graph.Graph;
import main.graph.GraphSearcher;
import main.graph.Solution;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class IDSSearcher<T> implements GraphSearcher<T> {
    private int maxDepth;

    public IDSSearcher(int maxDepth) {
        this.maxDepth = maxDepth;
    }

    /**
     * The recursive function of visiting nodes in the graph
     * @param graph - the given graph
     * @param src - the src node (current node we are at)
     * @param target - target node of the search
     * @param maxDepth - extra depth we have
     * @return List of nodes, which is created only when the target is found, and is being filled
     *          on the way back in the recursion.
     *          if path is not found - returns null.
     */
    private List<T> DLSVisit(Graph<T> graph, T src, T target, int maxDepth) {
        if (maxDepth == 0 && src.equals(target)) {
            // if found the goal path, create a path list and add the target
            List<T> path = new ArrayList<>();
            path.add(target);
            return path;
        }
        if (maxDepth <= 0) {
            return null;
        }
        List<T> neighbors = graph.getNeighbors(src);
        for (T v : neighbors) {
            List<T> path = DLSVisit(graph, v, target, maxDepth - 1);
            // if found a path
            if (path != null) {
                // add the current node to it and return
                path.add(src);
                return path;
            }
        }
        return null;
    }

    public Solution<T> search(Graph<T> graph) {
        Map<T, T> parent = new HashMap<>();
        parent.put(graph.getStart(), null);

        // basiclly, just implementation of the pseudo code from wikipedia.
        // iterative deepening depth first search
        for (int depth = 1; depth < maxDepth; ++depth) {
            List<T> path = DLSVisit(graph, graph.getStart(), graph.getGoal(), depth);
            // if found a path
            if (path != null) {
                // put the path in the parent map, used to return a solution
                for (int i = 1; i < path.size(); ++i) {
                    parent.put(path.get(i - 1), path.get(i));
                }
                // return the solution
                return new Solution<>(parent);
            }
        }
        // return an empty (except start node) solution.
        return new Solution<>(parent);
    }
}
