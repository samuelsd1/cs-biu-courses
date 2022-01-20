import java.util.Map;

/**
 * @author David Samuelson.
 */
public class Cos extends UnaryExpression implements Expression {

    /**
     * Constructor - constructs new Cos from a given number.
     *
     * @param num
     *            - the number
     */
    public Cos(double num) {
        super(new Num(num));
    }

    /**
     * Constructor - constructs new Cos from a given variable.
     *
     * @param var
     *            - the variable
     */
    public Cos(String var) {
        super(new Var(var));
    }

    /**
     * Constructor - constructs new Cos from a given expression.
     *
     * @param exp
     *            - the expression
     */
    public Cos(Expression exp) {
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
        return Math.cos(Math.toRadians(super.getExp().evaluate(assignment)));
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
        return Math.cos(Math.toRadians(super.getExp().evaluate()));
    }

    @Override
    public Expression assign(String var, Expression expression) {
        return new Cos(super.getExp().assign(var, expression));
    }

    /**
     * Returns a nice string representation of the expression.
     *
     * @return - nice string representation of the expression.
     */
    @Override
    public String toString() {
        return "cos(" + super.getExp().toString() + ")";
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
        return new Neg(new Mult(new Sin(super.getExp()), super.getExp().differentiate(var)));
    }

    /**
     * @return simplified version of the current expression.
     */
    @Override
    public Expression simplify() {
        // new cos with inner simplified parts
        Expression simplified = new Cos(super.getExp().simplify());
        try {
            // if i can evaluate, return it
            return new Num(simplified.evaluate());
        } catch (Exception e) {
            // couldnt evaluate, just return the simplified cos.
            return simplified;
        }
    }
}