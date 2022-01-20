#ifndef AP1_EX1_POINT_H
#define AP1_EX1_POINT_H

#include <ostream>
#include <boost/serialization/access.hpp>

/**
 * Point is a class that represents a point in the x,y plane.
 * holds two integers, x and y.
 */
class Point {
public:
    /**
    * Point default constsructor
    * @return Point at (0,0)
    */
    Point();
    Point(int xVal, int yVal);

    int getX() const;
    int getY() const;

    void move(int dx, int dy);
    void moveTo(const Point & p);
    bool operator==(const Point &right);
    Point operator+(const Point &right);
    Point operator-(const Point &right);

    /**
    * Print operator overloading.
    * @param output - the output stream to write the point string representation to.
    * @param P - the point
    * @return the updated stream after writing the point string representation to it.
    */
    friend std::ostream &operator<<(std::ostream &output, const Point &P);

private:
    friend class boost::serialization::access;
    // When the class Archive corresponds to an output archive, the
    // & operator is defined similar to <<.  Likewise, when the class Archive
    // is a type of input archive the & operator is defined similar to >>.
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & x;
        ar & y;
    }

    int x, y;
};


#endif //AP1_EX1_POINT_H
