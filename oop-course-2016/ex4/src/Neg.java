import java.util.Map;

/**
 * @author David Samuelson.
 */
public class Neg extends UnaryExpression implements Expression {

    /**
     * Constructor - constructs new Neg expression from a double value.
     *
     * @param num
     *            - a double value.
     */
    public Neg(double num) {
        super(new Num(num));
    }

    /**
     * Constructor - constructs new Neg expression from a string.
     *
     * @param var
     *            - String, represents variable
     */
    public Neg(String var) {
        super(new Var(var));
    }

    /**
     * Constructor - constructs new Neg expression from an expression.
     *
     * @param exp
     *            - the expression.
     */
    public Neg(Expression exp) {
        super(exp);
    }

    /**
     * Evaluate the expression using the variable values provided in the
     * assignment, and return the result. If the expression contains a variable
     * which is not in the assignment, an exception is thrown.
     *
     * @param assignment
     *            - Map that providing variable and value to assign.
     * @return - result of the evaluated expresion after assignment
     * @throws Exception
     *             - if a variable that wasnt in the assignment is inside the
     *             expression.
     */
    @Override
    public double evaluate(Map<String, Double> assignment) throws Exception {
        return -super.getExp().evaluate(assignment);
    }

    /**
     * Try to evaluate the expression without assigning any variables. if
     * expression contains variables - it throws exception. A convenience
     * method. Like the `evaluate(assignment)` method above, but uses an empty
     * assignment.
     *
     * @return - the expression evaluated.
     * @throws Exception
     *             - if couldnt evaluate the expression.
     */
    @Override
    public double evaluate() throws Exception {
        return -super.getExp().evaluate();
    }

    /**
     * Returns a new expression in which all occurrences of the variable var are
     * replaced with the provided expression (Does not modify the current
     * expression).
     *
     * @param var
     *            - the variable we replace with the expression.
     * @param expression
     *            - the expression we replace var with.
     * @return - a new Expression.
     */
    @Override
    public Expression assign(String var, Expression expression) {
        return new Neg(super.getExp().assign(var, expression));
    }

    /**
     * Returns a nice string representation of the expression.
     *
     * @return - nice string representation of the expression.
     */
    @Override
    public String toString() {
        return "(-" + super.getExp().toString() + ")";
    }

    /**
     * Returns the expression tree resulting from differentiating the current
     * expression relative to variable `var`.
     *
     * @param var
     *            - the variable we differentiate the expression by.
     * @return - the expression tree resulting from differentiating the current
     *         expression relative to variable `var`.
     */
    @Override
    public Expression differentiate(String var) {
        return new Neg(super.getExp().differentiate(var));
    }

    /**
     * @return simplified version of the current expression.
     */
    @Override
    public Expression simplify() {
        // same neg but with inner part simplified
        Expression simplified = new Neg(super.getExp().simplify());
        try {
            // try to evaluate
            return new Num(simplified.evaluate());
        } catch (Exception e) {
            // couldnt evaluate, returning just the simplified neg
            return simplified;
        }
    }
}
