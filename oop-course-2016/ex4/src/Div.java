import java.util.Map;

/**
 * @author David Samuelson.
 */
public class Div extends BinaryExpression implements Expression {

    /**
     * Constructor - constructs a new Div Expression from 2 given numbers.
     *
     * @param x
     *            - the first number
     * @param y
     *            - the second number
     */
    public Div(double x, double y) {
        super(new Num(x), new Num(y));
    }

    /**
     * Constructor - constructs a new Div Expression from a given number and a
     * variable.
     *
     * @param num
     *            - the number
     * @param var
     *            - the variable
     */
    public Div(double num, String var) {
        super(new Num(num), new Var(var));
    }

    /**
     * Constructor - constructs a new Div Expression from a given number and a
     * expression.
     *
     * @param num
     *            - the number
     * @param exp
     *            - the expression
     */
    public Div(double num, Expression exp) {
        super(new Num(num), exp);
    }

    /**
     * Constructor - constructs a new Div Expression from a given variable and a
     * number.
     *
     * @param var
     *            - the variable
     * @param num
     *            - the number
     */
    public Div(String var, double num) {
        super(new Var(var), new Num(num));
    }

    /**
     * Constructor - constructs a new Div Expression from two given variables.
     *
     * @param var1
     *            - the first variable
     * @param var2
     *            - the second variable
     */
    public Div(String var1, String var2) {
        super(new Var(var1), new Var(var2));
    }

    /**
     * Constructor - constructs a new Div Expression from a given variable and
     * expression.
     *
     * @param var
     *            - the variable
     * @param exp
     *            - the expression
     */
    public Div(String var, Expression exp) {
        super(new Var(var), exp);
    }

    /**
     * Constructor - constructs a new Div Expression from a given expression and
     * number.
     *
     * @param x
     *            - the expression
     * @param num
     *            - the number
     */
    public Div(Expression x, double num) {
        super(x, new Num(num));
    }

    /**
     * Constructor - constructs a new Div Expression from a given expression and
     * variable.
     *
     * @param x
     *            - the expression
     * @param var
     *            - the variable
     */
    public Div(Expression x, String var) {
        super(x, new Var(var));
    }

    /**
     * Constructor - constructs a new Div Expression from two given expression.
     *
     * @param x
     *            - the first expression
     * @param y
     *            - the second expression
     */
    public Div(Expression x, Expression y) {
        super(x, y);
    }

    /**
     * functions returns the numerator of the divide expression.
     *
     * @return - the numerator of the divide expression.
     */
    public Expression getNumerator() {
        return super.getX();
    }

    /**
     * function returns the denominator of the divide expression.
     *
     * @return - the denominator of the divide expression
     */
    public Expression getDenominator() {
        return super.getY();
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
        return (this.getNumerator().evaluate(assignment) / this.getDenominator().evaluate(assignment));
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
        return (this.getNumerator().evaluate() / this.getDenominator().evaluate());
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
        // assign left, right and return new Div of assigned expressions
        Expression nominator = this.getNumerator().assign(var, expression);
        Expression denum = this.getDenominator().assign(var, expression);
        return new Div(nominator, denum);
    }

    /**
     * Returns a nice string representation of the expression.
     *
     * @return - nice string representation of the expression.
     */
    @Override
    public String toString() {
        return "(" + this.getNumerator().toString() + " / " + this.getDenominator().toString() + ")";
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
        // (f/g)' = (f'g - g'f)/g^2
        return new Div(
                new Minus(new Mult(this.getNumerator().differentiate(var), this.getDenominator()),
                        new Mult(this.getDenominator().differentiate(var), this.getNumerator())),
                new Pow(this.getDenominator(), 2));
    }

    /**
     * @return simplified version of the current expression.
     */
    @Override
    public Expression simplify() {
        Expression numSimplified = this.getNumerator().simplify();
        Expression denomSimplified = this.getDenominator().simplify();

        java.util.List<String> numVars = numSimplified.getVariables();
        java.util.List<String> denomVars = denomSimplified.getVariables();

        try {
            if (denomVars.isEmpty()) {
                if (numVars.isEmpty()) {
                    // if both denominator and numerator has no vars, calculate
                    // the expression
                    return new Num(this.evaluate());
                }
                // else, numerator has vars and denum doesnt have vars
                double denomEvaluated = denomSimplified.evaluate();
                // x/1 = x
                if (super.isDoubleEqual(denomEvaluated, 1d)) {
                    return numSimplified;
                }
            }
        } catch (Exception unimportant) {
            // couldnt evaluate, going to default case
            // trick to avoid checkstyle problems :)
            unimportant = null;
        }

        int cmp = numSimplified.toString().compareTo(denomSimplified.toString());
        // x/x = 1
        if (cmp == 0) {
            return new Num(1d);
        } else {
            // return simplified div
            return new Div(numSimplified, denomSimplified);
        }
    }

}
