#include "Point.h"
/**
 * Point default constsructor
 * @return Point at (0,0)
 */
Point::Point() {
    x = 0;
    y = 0;
}
/**
 * Point parameterized constructor
 * @param xVal - x value of the point
 * @param yVal - y value of the point
 * @return a point with the coordinates (xVal, yVal)
 */
Point::Point(int xVal, int yVal) {
    x=xVal;
    y=yVal;
}
/**
 * point's X getter
 * @return x value of the point
 */
int Point::getX() const {
    return x;
}
/**
 * Point's Y getter
 * @return y value of the point
 */
int Point::getY() const {
    return y;
}
/**
 * moves the point in dx, dy deltas
 * @param dx - the change in x coordinate.
 * @param dy - the change in y coordinate.
 */
void Point::move(int dx, int dy) {
    x += dx;
    y += dy;
}

/**
 * moves the current point to the point received
 * @param p
 */
void Point::moveTo(const Point &p) {
    x = p.x;
    y = p.y;
}

/**
 * operator overloading for the == operator
 * @param right - the other point we compare with.
 * @return true if the points are equal, false otherwise.
 */
bool Point::operator==(const Point &right) {
    return x == right.x && y == right.y;
}

/**
 * operator overloading for adding of points
 * @param right - the point added to the current one
 * @return a new point which its coords are addition between current and right point.
 */
Point Point::operator+(const Point &right) {
    Point p;
    p.x = x + right.x;
    p.y = y + right.y;
    return p;
}

/**
 * operator overloading for substracting of points
 * @param right - the point substracted to the current one
 * @return a new point which its coords are substraction between current point and right point.
 */
Point Point::operator-(const Point &right) {
    Point p;
    p.x = x - right.x;
    p.y = y - right.y;
    return p;
}

/**
 * Print operator overloading.
 * @param output - the output stream to write the point string representation to.
 * @param P - the point
 * @return the updated stream after writing the point string representation to it.
 */
std::ostream& operator<<(std::ostream &output, const Point &P) {
    output << "(" << P.x << "," << P.y << ")";
    return output;
}
