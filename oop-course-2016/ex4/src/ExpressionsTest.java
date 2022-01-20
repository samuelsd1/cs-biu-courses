import java.util.Map;
import java.util.TreeMap;

/**
 * @author David Samuelson.
 */
public class ExpressionsTest {
    /**
     * main function, creates an expression , and does what we were asked for.
     *
     * @param args
     *            - we dont use it.
     */
    public static void main(String[] args) {
        // create the desired expression
        Expression e = new Plus(new Mult(2, "x"), new Plus(new Sin(new Mult(4, "y")), new Pow("e", "x")));

        // print it
        System.out.println(e.toString());

        // create the assignment
        Map<String, Double> assignment = new TreeMap<String, Double>();
        assignment.put("x", 2d);
        assignment.put("y", 4d);
        assignment.put("e", 2.71d);

        // try to evaluate the expression with the assignment and print it
        try {
            System.out.println(e.evaluate(assignment));
        } catch (Exception e2) {
            e2.printStackTrace();
        }

        // differentiate the expression and put inside expDiff
        Expression expDiff = e.differentiate("x");
        // print the differentiated expression
        System.out.println(expDiff);

        // try to evaluate the differentiated expression with the assignment and
        // print it.
        try {
            System.out.println(expDiff.evaluate(assignment));
        } catch (Exception e2) {
            e2.printStackTrace();
        }

        // print the simplified differentiated expression
        System.out.println(expDiff.simplify());
    }
}
