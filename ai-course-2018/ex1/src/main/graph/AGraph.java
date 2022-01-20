package main.graph;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

/**
 * Abstract class that contains some utility functions for the graph.
 * @param <T>
 */
public abstract class AGraph<T> implements Graph<T> {

    /**
     * Computes the total cost of the given path
     * @param path - the path
     * @return the total cost of the given path
     */
    public double getTotalCost(List<T> path) {
        double totalCost = 0;
        for(int i=1;i<path.size();++i){
            totalCost += this.getCost(path.get(i-1),path.get(i));
        }
        return totalCost;
    }

    /**
     * Computes the path from the given solution
     * @param solution - the solution object
     * @return List of nodes that represents the path from start to goal.
     */
    public List<T> computePath(Solution<T> solution) {
        Map<T, T> parent = solution.getCameFrom();
        List<T> path = new ArrayList<>();
        for (T node = this.getGoal(); node != null; node = parent.get(node)) {
            path.add(0, node);
        }
        return path;
    }
}
