#include <gtest/gtest.h>
#include "../src/Core/Point.h"

#define POINT_MIN_VAL   -0xFFFF
#define POINT_MAX_VAL   0xFFFF

class PointTest : public testing::Test {
public:
    PointTest() {}

protected:
    Point p;
};

TEST_F(PointTest, Point_getX_test) {
    for (int i = POINT_MIN_VAL; i < POINT_MAX_VAL; i++) {
        p = (i == 0 ? Point(i, 1) : Point(i, 0)); // to cover a case that at x=0 it returns y.
        ASSERT_EQ(p.getX(), i) << "Point getX differs at x = " << i;
    }
}

TEST_F(PointTest, Point_getY_Test) {
    for (int i = POINT_MIN_VAL; i <= POINT_MAX_VAL; i++) {
        p = (i == 0 ? Point(1, i) : Point(0, i)); // to cover a case that at y=0 it returns x.
        ASSERT_EQ(p.getY(), i) << "Point getY() differs at y = " << i;
    }
}

TEST_F(PointTest, Point_equalsOverloading_Test) {
    for (int i = POINT_MIN_VAL; i <= POINT_MAX_VAL; i++) {
        Point p1 = Point(i, 0);
        Point p2 = Point(i, 0);
        ASSERT_EQ(p1 == p2, true);
        ASSERT_EQ(p2 == p1, true);
        p1 = Point(0, i);
        if (i != 0) {
            ASSERT_EQ(p1 == p2, false);
            ASSERT_EQ(p2 == p1, false);
        }
        p2 = Point(0, i);
        ASSERT_EQ(p1 == p2, true);
        ASSERT_EQ(p2 == p1, true);

    }
}

TEST_F(PointTest, Point_move_Test) {
    //combined x,y moves takes too much ( O(n^2), really takes too much time)
    for (int i = POINT_MIN_VAL; i <= POINT_MAX_VAL; i++) {
        p = Point(0, 0);
        p.move(i, 0);
        ASSERT_EQ(p.getX(), i); // check x value, getX tested before, we can count on that
        //reinit the point, to check y value move.
        p = Point(0, 0);
        p.move(0, i);
        ASSERT_EQ(p.getY(), i); // check y value, gety tested before, we can count on that
    }
}

TEST_F(PointTest, Point_PlusOverloading_Test) {
    for (int i = POINT_MIN_VAL; i <= POINT_MAX_VAL; i++) {
        Point p1 = Point(0, 0);
        Point p2 = Point(i, i);
        Point p3 = p1 + p2;
        ASSERT_EQ(p3 == Point(i, i), true);
    }
}

TEST_F(PointTest, Point_MinusOverloading_Test) {
    for (int i = POINT_MIN_VAL; i <= POINT_MAX_VAL; i++) {
        Point p1 = Point(0, 0);
        Point p2 = Point(i, i);
        p = p1 - p2;
        ASSERT_EQ(p == Point(-i, -i), true);
    }
}

TEST_F(PointTest, Point_PrintOverloading_Test) {
    for (int i = POINT_MIN_VAL; i <= POINT_MAX_VAL; i++) {
        //check x print
        p = Point(i, 0);
        std::stringstream pointPrint;
        std::stringstream check;
        pointPrint << p;
        check << "(" << i << "," << 0 << ")";
        ASSERT_EQ(pointPrint.str(), check.str());
        //check y print
        pointPrint.clear();
        check.clear();
        p = Point(0, i);
        pointPrint << p;
        check << "(" << 0 << "," << i << ")";
        ASSERT_EQ(pointPrint.str(), check.str());
    }
}