import java.util.Map;

/**
 * @author David Samuelson.
 */
public class Pow extends BinaryExpression implements Expression {

    /**
     * Constructor - constructs a new Pow Expression from 2 given numbers.
     *
     * @param base
     *            - the first number, the base of the power
     * @param exp
     *            - the second number, the exponent of the power
     */
    public Pow(double base, double exp) {
        super(new Num(base), new Num(exp));
    }

    /**
     * Constructor - constructs a new Pow Expression from a given number and a
     * variable.
     *
     * @param base
     *            - the number, the base of the power
     * @param exp
     *            - the variable, the exponent of the power
     */
    public Pow(double base, String exp) {
        super(new Num(base), new Var(exp));
    }

    /**
     * Constructor - constructs a new Pow Expression from a given number and a
     * expression.
     *
     * @param base
     *            - the number, the base of the power
     * @param exp
     *            - the expression, the exponent of the power
     */
    public Pow(double base, Expression exp) {
        super(new Num(base), exp);
    }

    /**
     * Constructor - constructs a new Pow Expression from a given variable and a
     * number.
     *
     * @param base
     *            - the variable, the base of the power
     * @param exp
     *            - the number, the exponent of the power
     */
    public Pow(String base, double exp) {
        super(new Var(base), new Num(exp));
    }

    /**
     * Constructor - constructs a new Pow Expression from two given variables.
     *
     * @param base
     *            - the first variable, the base of the power
     * @param exp
     *            - the second variable, the exponent of the power
     */
    public Pow(String base, String exp) {
        super(new Var(base), new Var(exp));
    }

    /**
     * Constructor - constructs a new Pow Expression from a given variable and
     * expression.
     *
     * @param base
     *            - the variable, the base of the power
     * @param exp
     *            - the expression, the exponent of the power
     */
    public Pow(String base, Expression exp) {
        super(new Var(base), exp);
    }

    /**
     * Constructor - constructs a new Pow Expression from a given expression and
     * number.
     *
     * @param base
     *            - the expression, the base of the power
     * @param exp
     *            - the number, the exponent of the power
     */
    public Pow(Expression base, double exp) {
        super(base, new Num(exp));
    }

    /**
     * Constructor - constructs a new Pow Expression from a given expression and
     * variable.
     *
     * @param base
     *            - the expression, the base of the power
     * @param exp
     *            - the variable, the exponent of the power
     */
    public Pow(Expression base, String exp) {
        super(base, new Var(exp));
    }

    /**
     * Constructor - constructs a new Pow Expression from two given expression.
     *
     * @param base
     *            - the first expression, the base of the power
     * @param exp
     *            - the second expression, the exponent of the power
     */
    public Pow(Expression base, Expression exp) {
        super(base, exp);
    }

    /**
     * function returns the base of the power.
     *
     * @return the base of the Pow expression
     */
    public Expression getBase() {
        return super.getX();
    }

    /**
     * function returns the exponent of the power.
     *
     * @return the exponent of the Pow expression
     */
    public Expression getExponent() {
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
        return Math.pow(this.getBase().evaluate(assignment), this.getExponent().evaluate(assignment));
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
        return Math.pow(this.getBase().evaluate(), this.getExponent().evaluate());
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
        // assign base, exponent and return new Pow of assigned expressions
        Expression base = this.getBase().assign(var, expression);
        Expression exp = this.getExponent().assign(var, expression);
        return new Pow(base, exp);
    }

    /**
     * Returns a nice string representation of the expression.
     *
     * @return - nice string representation of the expression.
     */
    @Override
    public String toString() {
        return "(" + this.getBase().toString() + "^" + this.getExponent().toString() + ")";
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
        // according to wikipedia, (f^g)' = f^g*(f'*(g/f) + g'*ln f)

        java.util.List<String> baseVars = this.getBase().getVariables();
        java.util.List<String> exponentVars = this.getExponent().getVariables();

        Expression baseDiff = this.getBase().differentiate(var);
        Expression expDiff = this.getExponent().differentiate(var);

        // if base contaians the var we diff by
        if (baseVars.contains(var)) {
            // if also exponent contains the var we diff by
            if (exponentVars.contains(var)) {
                // its (f^g)' = f^g(f' * (g/f) + g' * ln f)
                return new Mult(this, new Plus(new Mult(baseDiff, new Div(this.getExponent(), this.getBase())),
                        new Mult(expDiff, new Log("e", this.getBase()))));
            }
            // if exponent is not our var, like f(x)^a , then diff is a* f^(a-1)
            return new Mult(new Mult(this.getExponent(), new Pow(this.getBase(), new Minus(this.getExponent(), 1))),
                    baseDiff);
        } else {
            if (exponentVars.contains(var)) {
                // if its a^f(x), then diff is a^f(x) * ln(a) * f'(x)
                return new Mult(new Mult(this, new Log("e", this.getBase())), expDiff);
            }
            // else, if expression contains no vars, then its a constant. return
            // 0;
            return new Num(0);
        }
    }

    /**
     * @return simplified version of the current expression.
     */
    @Override
    public Expression simplify() {
        // simplify base and exponent
        Expression baseSimplified = this.getBase().simplify();
        Expression exponentSimplified = this.getExponent().simplify();

        // variaables of base and component
        java.util.List<String> baseVars = baseSimplified.getVariables();
        java.util.List<String> exponentVars = exponentSimplified.getVariables();
        try {
            // if exponent has no vars
            if (exponentVars.isEmpty()) {
                // if the base has no vars
                if (baseVars.isEmpty()) {
                    // return the expression evaluated
                    return new Num(new Pow(baseSimplified, exponentSimplified).evaluate());
                }
                // exponentVars empty, baseVars not empty. calculate exponent
                // and see if its zero.
                double exponentEvaluated = exponentSimplified.evaluate();
                if (super.isDoubleEqual(exponentEvaluated, 0)) {
                    // exponent is zero, then evaluated value is 1
                    return new Num(1d);
                }
            } else {
                // exponent has vars, then we check the base
                double baseEvaluated = baseSimplified.evaluate();
                if (super.isDoubleEqual(baseEvaluated, 1d)) {
                    // if the base is 1, the evaluated value is 1
                    return new Num(1d);
                }
            }
        } catch (Exception unimportant) {
            // evaluate didnt suscced, going to default case.
            // trick to avoid checkstyle problems :)
            unimportant = null;
        }
        // returning the pow, but simplified
        return new Pow(baseSimplified, exponentSimplified);
    }

}
