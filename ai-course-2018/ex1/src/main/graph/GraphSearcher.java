package main.graph;

// self explanatory
public interface GraphSearcher<T> {
    Solution<T> search(Graph<T> graph);
}
