import java.util.List;

/**
 * @author David Samuelson.
 */
public abstract class BinaryExpression extends BaseExpression {

    private Expression x;
    private Expression y;

    /**
     * returns the first expression in the binaryExpression.
     *
     * @return the first expression in the binaryExpression
     */
    protected final Expression getX() {
        return this.x;
    }

    /**
     * returns the second expression in the binaryExpression.
     *
     * @return the second expression in the binaryExpression
     */
    protected final Expression getY() {
        return this.y;
    }

    /**
     * @param exp1
     *            - first expression
     * @param exp2
     *            - second expression
     */
    public BinaryExpression(Expression exp1, Expression exp2) {
        this.x = exp1;
        this.y = exp2;
    }

    /**
     * Returns a list of the variables in the expression.
     *
     * @return a list of the variables in the expression.
     */
    public List<String> getVariables() {
        List<String> xVars = this.x.getVariables();
        List<String> yVars = this.y.getVariables();

        // look on the y expression variables
        for (String var : yVars) {
            // if x vars doesnt contain the y var we checking, add the var to
            // the x Vars. then finally return x vars
            if (!xVars.contains(var)) {
                // add the var to x vars
                xVars.add(var);
            }
        }

        // return the complete list holding all the variables
        return xVars;
    }

}
