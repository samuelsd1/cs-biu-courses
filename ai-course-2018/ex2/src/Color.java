/**
 * Name:    David Samuelson
 * ID:      208788851
 * Uname:   samueld1
 */
/**
 * player enum
 */
public enum Color {
    EMPTY,
    BLACK,
    WHITE;

    public static Color getOpponent(Color p){
        if(p == EMPTY) throw new RuntimeException("No opponent for EMPTY");
        return (p == BLACK) ? WHITE : BLACK;
    }
}