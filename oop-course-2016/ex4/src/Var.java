import java.util.List;
import java.util.Map;

/**
 * @author David Samuelson.
 */
public class Var implements Expression {

    private String variable;

    /**
     * Constructor - constructs new Var instance.
     *
     * @param var
     *            - a string that presents the variable
     */
    public Var(String var) {
        this.variable = var;
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
        // if the assignment has value for the var, return the value
        if (assignment.containsKey(variable)) {
            return assignment.get(variable);
        } else {
            // else, the assignment does not handle our var, and we cant assign.
            throw new Exception("cannot evaluate unassigned var");
        }
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
        // no assignment - no return
        throw new Exception("cannot evaluate unassigned var");
    }

    /**
     * Returns a list of the variables in the expression.
     *
     * @return a list holding the variable.
     */
    @Override
    public List<String> getVariables() {
        List<String> variables = new java.util.ArrayList<String>();
        variables.add(this.variable);
        return variables;
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
        if (variable.equals(var)) {
            return expression;
        }
        return this;
    }

    /**
     * Returns a nice string representation of the expression.
     *
     * @return - nice string representation of the expression.
     */
    @Override
    public String toString() {
        return this.variable;
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
        if (this.variable.equals(var)) {
            return new Num(1);
        }
        return new Num(0);
    }

    /**
     * @return simplified version of the current expression.
     */
    @Override
    public Expression simplify() {
        // var has no simplifications
        return this;
    }

}
