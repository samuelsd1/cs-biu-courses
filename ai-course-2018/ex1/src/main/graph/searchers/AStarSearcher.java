package main.graph.searchers;

import main.util.BetterPriorityQueue;
import main.graph.Graph;
import main.graph.GraphSearcher;
import main.graph.Heuristic;
import main.graph.Solution;

import java.util.HashMap;
import java.util.Map;


public class AStarSearcher<T> implements GraphSearcher<T> {
    private Heuristic<T> heuristic;

    public AStarSearcher(Heuristic<T> heuristic) {
        this.heuristic = heuristic;
    }

    public Solution<T> search(Graph<T> graph) {
        BetterPriorityQueue<T> frontier = new BetterPriorityQueue<>();

        T start = graph.getStart();
        T goal = graph.getGoal();

        frontier.put(start, 0.0);

        Map<T, T> parent = new HashMap<>();
        Map<T, Double> costSoFar = new HashMap<>();

        parent.put(start, null);
        costSoFar.put(start, 0.0);

        while (!frontier.isEmpty()) {
            T current = frontier.get();

            if (current.equals(goal)) {
                break;
            }

            // for each neighbor
            for (T next : graph.getNeighbors(current)) {
                // Duplicate Pruning
                if (frontier.contains(next)) {
                    continue;
                }

                // cost of the new node (neighbor)
                double new_cost = costSoFar.get(current) + graph.getCost(current, next);
                // if the new cost is better than what we know
                if (new_cost < costSoFar.getOrDefault(next, (double)Integer.MAX_VALUE)) {
                    // 1.update the neighbor (parent & cost)
                    // 2.put it back into the frontier
                    parent.put(next, current);
                    costSoFar.put(next, new_cost);
                    double priority = new_cost + this.heuristic.getHeuristic(next, goal);
                    frontier.put(next, priority);
                }
            }
        }
        return new Solution<>(parent);
    }
}
