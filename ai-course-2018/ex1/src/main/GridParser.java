package main;

import main.graph.AlgorithmType;

import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;

/**
 * Just a code file that stores the parsing function
 */
public class GridParser {

    /**
     * Reads the input file, parses it and returns the result
     * @param filename
     * @return
     * @throws Exception - in any case of problem in the format of the file,
     *              and/or IO Exception - I just pass it forward.
     */
    public static ParseResult ParseGrid(String filename) throws Exception{
        List<String> lines = Files.readAllLines(Paths.get(filename));

        // not enough lines - bad format.
        if(lines.size() < 3){
            throw new Exception("Unknown Format - Received less than 3 lines.");
        }

        // self explanatory
        AlgorithmType type;
        switch(lines.get(0)){
            case "IDS":
                type = AlgorithmType.IDS;
                break;
            case "A*":
                type = AlgorithmType.AStar;
                break;
            default:
                throw new Exception("Unknown algorithm \"" + lines.get(0) + "\"");
        }

        int n = Integer.parseInt(lines.get(1));

        Cells[][] grid = new Cells[n][n];

        lines.remove(1);
        lines.remove(0);

        // just parse that matrix and insert everything to the grid
        for(int row=0;row<n;++row){
            String line = lines.get(row);
            for(int col=0;col<n;++col){
                switch(line.charAt(col)){
                    case 'S':
                        grid[row][col] = Cells.START;
                        break;
                    case 'G':
                        grid[row][col] = Cells.GOAL;
                        break;
                    case 'R':
                        grid[row][col] = Cells.ROAD;
                        break;
                    case 'D':
                        grid[row][col] = Cells.DIRT;
                        break;
                    case 'H':
                        grid[row][col] = Cells.HILL;
                        break;
                    case 'W':
                        grid[row][col] = Cells.WATER;
                        break;
                    default:
                        throw new Exception("Unsupported Cell Type '" + line.charAt(col) + "'");
                }
            }
        }

        return new ParseResult(type,grid);

    }
}
