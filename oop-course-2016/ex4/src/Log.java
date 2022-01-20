import java.util.Map;

/**
 * @author David Samuelson.
 */
public class Log extends BinaryExpression implements Expression {

    /**
     * Constructor - constructs a new Log Expression from 2 given numbers.
     *
     * @param base
     *            - the first number, base of the logarithm
     * @param x
     *            - the second number
     */
    public Log(double base, double x) {
        super(new Num(base), new Num(x));
    }

    /**
     * Constructor - constructs a new Log Expression from a given number and a
     * variable.
     *
     * @param base
     *            - the number, base of the logarithm
     * @param var
     *            - the variable
     */
    public Log(double base, String var) {
        super(new Num(base), new Var(var));
    }

    /**
     * Constructor - constructs a new Log Expression from a given number and a
     * expression.
     *
     * @param base
     *            - the number, base of the logarithm
     * @param exp
     *            - the expression
     */
    public Log(double base, Expression exp) {
        super(new Num(base), exp);
    }

    /**
     * Constructor - constructs a new Log Expression from a given variable and a
     * number.
     *
     * @param base
     *            - the variable, base of the logarithm
     * @param num
     *            - the number
     */
    public Log(String base, double num) {
        super(new Var(base), new Num(num));
    }

    /**
     * Constructor - constructs a new Log Expression from two given variables.
     *
     * @param base
     *            - the first variable, base of the logarithm
     * @param var
     *            - the second variable
     */
    public Log(String base, String var) {
        super(new Var(base), new Var(var));
    }

    /**
     * Constructor - constructs a new Log Expression from a given variable and
     * expression.
     *
     * @param base
     *            - the variable, base of the logarithm
     * @param exp
     *            - the expression
     */
    public Log(String base, Expression exp) {
        super(new Var(base), exp);
    }

    /**
     * Constructor - constructs a new Log Expression from a given expression and
     * number.
     *
     * @param base
     *            - the expression
     * @param num
     *            - the number
     */
    public Log(Expression base, double num) {
        super(base, new Num(num));
    }

    /**
     * Constructor - constructs a new Log Expression from a given expression and
     * variable.
     *
     * @param base
     *            - the expression
     * @param var
     *            - the variable
     */
    public Log(Expression base, String var) {
        super(base, new Var(var));
    }

    /**
     * Constructor - constructs a new Log Expression from two given expression.
     *
     * @param base
     *            - the first expression
     * @param x
     *            - the second expression
     */
    public Log(Expression base, Expression x) {
        super(base, x);
    }

    /**
     * @return the base of the logarithm
     */
    public Expression getBase() {
        return super.getX();
    }

    /**
     * @return the expression we do log on.
     */
    public Expression getExp() {
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
        // assign and calculate log using the log function below.
        return this.log(this.getBase().evaluate(assignment), this.getExp().evaluate(assignment));
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
        // calculate log using the log function below.
        return this.log(this.getBase().evaluate(), this.getExp().evaluate());
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
        // assign to base and to the exp and return new log.
        Expression base = this.getBase().assign(var, expression);
        Expression x = this.getExp().assign(var, expression);
        return new Log(base, x);
    }

    /**
     * Returns a nice string representation of the expression.
     *
     * @return - nice string representation of the expression.
     */
    @Override
    public String toString() {
        return "log(" + this.getBase().toString() + " ," + this.getExp().toString() + ")";
    }

    /**
     * function gets base and number for log, and calculates it.
     *
     * @param base
     *            - the base of the logarithm
     * @param x
     *            - the number we do log on
     * @return - the logarithm calculated.
     */
    private double log(double base, double x) {
        return Math.log10(x) / Math.log10(base);
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
        java.util.List<String> baseVars = this.getBase().getVariables();
        java.util.List<String> exptVars = this.getExp().getVariables();

        // if base of the log contains logarithm
        if (baseVars.contains(var)) {
            // log (x,x), then transfer the log to ln and differentiate
            Expression newLog = new Div(new Log("e", this.getExp()), new Log("e", this.getBase()));
            return newLog.differentiate(var);
        } else {
            // else - if the base does not contain any vars

            // if the exponent contains vars
            if (exptVars.contains(var)) {
                // if its log(b,x), differentiate by formula f' / (f*ln(b))
                return new Div(this.getExp().differentiate(var), new Mult(this.getExp(), new Log("e", this.getBase())));
            }
            // else, if its log(b,b), then its const, return 0;
            return new Num(0);
        }
    }

    /**
     * @return simplified version of the current expression.
     */
    @Override
    public Expression simplify() {
        Expression baseSimplified = this.getBase().simplify();
        Expression expSimplified = this.getExp().simplify();

        // simplified log
        Expression simplified = new Log(baseSimplified, expSimplified);

        try {
            // try to evaluate the log.
            return new Num(simplified.evaluate());
        } catch (Exception unimportant) {
            // it couldnt evalutate, continue to default case
            // trick to avoid checkstyle problems :)
            unimportant = null;
        }

        // if base and the exp are equal then log evaluate is 1;
        int cmp = baseSimplified.toString().compareTo(expSimplified.toString());
        if (cmp == 0) {
            return new Num(1d);
        } else {
            // else, just return new log with simplified inner parts.
            return simplified;
        }
    }

}
