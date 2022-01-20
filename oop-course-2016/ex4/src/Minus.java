import java.util.Map;

/**
 * @author David Samuelson.
 */
public class Minus extends BinaryExpression implements Expression {

    /**
     * Constructor - constructs a new Minus Expression from 2 given numbers.
     *
     * @param x
     *            - the first number
     * @param y
     *            - the second number
     */
    public Minus(double x, double y) {
        super(new Num(x), new Num(y));
    }

    /**
     * Constructor - constructs a new Minus Expression from a given number and a
     * variable.
     *
     * @param num
     *            - the number
     * @param var
     *            - the variable
     */
    public Minus(double num, String var) {
        super(new Num(num), new Var(var));
    }

    /**
     * Constructor - constructs a new Minus Expression from a given number and a
     * expression.
     *
     * @param num
     *            - the number
     * @param exp
     *            - the expression
     */
    public Minus(double num, Expression exp) {
        super(new Num(num), exp);
    }

    /**
     * Constructor - constructs a new Minus Expression from a given variable and
     * a number.
     *
     * @param var
     *            - the variable
     * @param num
     *            - the number
     */
    public Minus(String var, double num) {
        super(new Var(var), new Num(num));
    }

    /**
     * Constructor - constructs a new Minus Expression from two given variables.
     *
     * @param var1
     *            - the first variable
     * @param var2
     *            - the second variable
     */
    public Minus(String var1, String var2) {
        super(new Var(var1), new Var(var2));
    }

    /**
     * Constructor - constructs a new Minus Expression from a given variable and
     * expression.
     *
     * @param var
     *            - the variable
     * @param exp
     *            - the expression
     */
    public Minus(String var, Expression exp) {
        super(new Var(var), exp);
    }

    /**
     * Constructor - constructs a new Minus Expression from a given expression
     * and number.
     *
     * @param x
     *            - the expression
     * @param num
     *            - the number
     */
    public Minus(Expression x, double num) {
        super(x, new Num(num));
    }

    /**
     * Constructor - constructs a new Minus Expression from a given expression
     * and variable.
     *
     * @param x
     *            - the expression
     * @param var
     *            - the variable
     */
    public Minus(Expression x, String var) {
        super(x, new Var(var));
    }

    /**
     * Constructor - constructs a new Minus Expression from two given
     * expression.
     *
     * @param x
     *            - the first expression
     * @param y
     *            - the second expression
     */
    public Minus(Expression x, Expression y) {
        super(x, y);
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
        return (super.getX().evaluate(assignment) - super.getY().evaluate(assignment));
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
        return (super.getX().evaluate() - super.getY().evaluate());
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
        // assign left, right and return new Minus of assigned expressions
        Expression left = super.getX().assign(var, expression);
        Expression right = super.getY().assign(var, expression);
        return new Minus(left, right);
    }

    /**
     * Returns a nice string representation of the expression.
     *
     * @return - nice string representation of the expression.
     */
    @Override
    public String toString() {
        return "(" + super.getX().toString() + " - " + super.getY().toString() + ")";
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
        // (f-g)' = f' - g'
        return new Minus(super.getX().differentiate(var), super.getY().differentiate(var));
    }

    /**
     * @return simplified version of the current expression.
     */
    @Override
    public Expression simplify() {
        // left and right expressions simplified
        Expression leftSimplified = super.getX().simplify();
        Expression rightSimplified = super.getY().simplify();

        // left and right variables
        java.util.List<String> leftVars = leftSimplified.getVariables();
        java.util.List<String> rightVars = rightSimplified.getVariables();

        try {
            // if left side has no variables.
            if (leftVars.isEmpty()) {
                // if right side has no variables.
                if (rightVars.isEmpty()) {
                    // if we can evaluate, then we will return value;
                    return new Num(new Minus(leftSimplified, rightSimplified).evaluate());
                }
                // else, right side has vars
                double leftEvaluate = leftSimplified.evaluate();
                // 0 - EXP = -EXP
                if (super.isDoubleEqual(leftEvaluate, 0d)) {
                    return new Neg(rightSimplified);
                }
            } else {
                // if left side does have variables
                if (rightVars.isEmpty()) {
                    double rightEvaluate = rightSimplified.evaluate();
                    // EXP - 0 = EXP
                    if (super.isDoubleEqual(rightEvaluate, 0d)) {
                        return leftSimplified;
                    }
                }
            }
        } catch (Exception unimportant) {
            // something happened while evaluating, continue to default case
            // trick to avoid checkstyle problems :)
            unimportant = null;
        }
        // compare the strings of the expressions, supposed to be equal because
        // thats how the code sorted them before.
        int cmp = leftSimplified.toString().compareTo(rightSimplified.toString());
        // EXP - EXP = 0
        if (cmp == 0) {
            return new Num(0d);
        } else {
            // just return minus with inner expressions simplified
            return new Minus(leftSimplified, rightSimplified);
        }
    }
}
