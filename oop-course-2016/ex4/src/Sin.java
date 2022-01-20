import java.util.Map;

/**
 * @author David Samuelson.
 */
public class Sin extends UnaryExpression implements Expression {

    /**
     * Constructor - constructs new Sin instance.
     *
     * @param num
     *            - a double value, representing just a number
     */
    public Sin(double num) {
        super(new Num(num));
    }

    /**
     * Constructor - constructs new Sin instance.
     *
     * @param var
     *            - a string, respresenting variable
     */
    public Sin(String var) {
        super(new Var(var));
    }

    /**
     * Constructor - constructs new Sin instance.
     *
     * @param exp
     *            - an expression
     */
    public Sin(Expression exp) {
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
        // assign and evaluate the sin
        return Math.sin(Math.toRadians(super.getExp().evaluate(assignment)));

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
        // evaluate the sin
        return Math.sin(Math.toRadians(super.getExp().evaluate()));
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
        // return new sin of inner part assigned
        return new Sin(super.getExp().assign(var, expression));
    }

    /**
     * Returns a nice string representation of the expression.
     *
     * @return - nice string representation of the expression.
     */
    @Override
    public String toString() {
        return "sin(" + super.getExp().toString() + ")";
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
        return new Mult(new Cos(super.getExp()), super.getExp().differentiate(var));
    }

    /**
     * @return simplified version of the current expression.
     */
    @Override
    public Expression simplify() {
        // new Sinus, but with inner part simplified.
        Expression simplified = new Sin(super.getExp().simplify());
        try {
            // if we can evaluate, return evaluated num.
            return new Num(simplified.evaluate());
        } catch (Exception e) {
            // we couldnt evaluate, so we just returned the simplified sin.
            return simplified;
        }
    }
}
