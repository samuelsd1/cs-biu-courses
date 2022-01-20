package main.graph;

// self explanatory
public interface Heuristic<T> {
    double getHeuristic(T src, T target);
}
