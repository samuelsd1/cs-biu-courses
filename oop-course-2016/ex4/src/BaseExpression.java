/**
 * @author David Samuelson.
 */
public abstract class BaseExpression {
    private final double errorMargin = 0.000000001;

    /**
     * @param a
     *            - first double
     * @param b
     *            - second double
     * @return boolaen, respresenting if the double values are equal
     */
    protected boolean isDoubleEqual(double a, double b) {
        // took this double compare value from stack overflaw
        return Math.abs(a - b) <= errorMargin;
    }
}
