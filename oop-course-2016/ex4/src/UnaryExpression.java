import java.util.List;

/**
 * @author David Samuelson.
 */
public abstract class UnaryExpression extends BaseExpression {
    private Expression x;

    /**
     * function returns the inner expression of the unary expression.
     *
     * @return - the inner expression of the unary expression
     */
    protected Expression getExp() {
        return this.x;
    }

    /**
     * Constructor - constructs new UnaryExpression instance.
     *
     * @param exp
     *            - an expression
     */
    public UnaryExpression(Expression exp) {
        this.x = exp;
    }

    /**
     * Returns a list of the variables in the expression.
     *
     * @return a list of the variables in the expression.
     */
    public List<String> getVariables() {
        return this.x.getVariables();
    }
}
