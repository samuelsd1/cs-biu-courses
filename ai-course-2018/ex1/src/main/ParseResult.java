package main;

import main.graph.AlgorithmType;

/**
 * Class used to store the result from the input parser, because its not python and I dont have tuples.
 */
public class ParseResult {
    private AlgorithmType type;
    private Cells[][] grid;

    public ParseResult(AlgorithmType type, Cells[][] grid){
        this.type = type;
        this.grid = grid;
    }

    public AlgorithmType getType(){
        return this.type;
    }

    public Cells[][] getGrid(){
        return this.grid;
    }
}
