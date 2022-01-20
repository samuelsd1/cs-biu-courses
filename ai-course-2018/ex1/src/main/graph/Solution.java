package main.graph;

import java.util.Map;

/**
 * Stupid class for saving a solution. Its here just to make a better design of the code, and make it cleaner.
 * @param <T> - the type of the nodes that the solution contains.
 */
public class Solution<T> {
    private Map<T,T> cameFrom;
    public Solution(Map<T,T> cameFrom){
        this.cameFrom = cameFrom;
    }

    public Map<T,T> getCameFrom(){
        return this.cameFrom;
    }
}
