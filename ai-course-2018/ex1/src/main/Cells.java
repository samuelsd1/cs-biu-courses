package main;

/**
 * Just a supercool enum, I love java enums :D
 */
public enum Cells {
    START   (1),
    GOAL    (0),
    ROAD    (1),
    DIRT    (3),
    HILL    (10),
    WATER   (Integer.MAX_VALUE)
    ;

    private final double cost;
    Cells(double cost){
        this.cost = cost;
    }
    public double cost(){
        return this.cost;
    }
}
