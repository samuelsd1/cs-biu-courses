package main.graph;

import java.util.List;

// self explanatory
public interface Graph<T> {
    List<T> getNeighbors(T v);

    T getStart();

    T getGoal();

    double getCost(T src, T target);
}
