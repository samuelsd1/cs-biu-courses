#include <gtest/gtest.h>
#include "../src/Core/Vehicles/Taxi.h"

#define POINT_BOUND (0xFF)

class VehicleTest : public testing::Test {
public:
    VehicleTest() {}
};

TEST_F(VehicleTest, Vehicle_move_Test) {
    //check the move to that point
    Vehicle *v = new Taxi(Point(0, 0), 0, 0, 1,HONDA, WHITE);
    for (int i = -POINT_BOUND; i <= POINT_BOUND; i++) {
        for (int j = -POINT_BOUND; j <= POINT_BOUND; j++) {
            Point currentPoint = Point(i, j);
            //expected to move to the right location.
            v->move(currentPoint);
            EXPECT_TRUE(currentPoint == v->getLocation());
        }
    }

}