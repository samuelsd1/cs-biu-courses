/**
 * Name:    David Samuelson
 * ID:      208788851
 * Uname:   samueld1
 */
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.List;

/**
 * Parses the input
 */
public class InputParser {


    /**
     * Parses the input
     * @param path path to the file
     * @return the board from the file
     * @throws Exception - in case of bad format, or file not found
     */
    public static Board parseInput(String path) throws Exception{
        Path p = Paths.get(path);
        List<String> lines = Files.readAllLines(Paths.get(path));
        if(lines.size() != 5)
            throw new Exception("Bad input format: wrong number of lines");

        Board b = new Board();
        for(int row=0;row<5;++row){
            for(int col = 0;col<5;++col){
                char c = lines.get(row).charAt(col);
                Color cell;
                switch(c){
                    case 'E':
                        cell = Color.EMPTY;
                        break;
                    case 'B':
                        cell = Color.BLACK;
                        break;
                    case 'W':
                        cell = Color.WHITE;
                        break;
                    default:
                        throw new Exception("Bad input format: unknown symbol " + c);
                }
                b.setCell(row,col,cell);
            }
        }
        return b;
    }
}
