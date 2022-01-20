#include <gtest/gtest.h>
#include "../src/Core/Point.h"
#include "../src/Core/Trip/TripInfo.h"

using namespace std;

class TripInfoTest : public testing::Test {
public:
    TripInfoTest() {}

protected:
    TripInfo t;
};

TEST_F(TripInfoTest, Trip_getRideID_test) {
    Point p = Point();
    for (unsigned int id = 0; id < 0xFFFF; id++) {
        t = TripInfo(id, 0, p, p, 0, 0,0);
        ASSERT_EQ(t.getRideID(), id) << "TripInfo getRideID differs at id = " << id;
    }
}

TEST_F(TripInfoTest, Trip_getTotalMeters_test) {
    Point p = Point();
    for (unsigned int i = 0; i < 0xFFFF; i++) {
        t = TripInfo(0, i, p, p, 0, 0,0);
        ASSERT_EQ(t.getTotalMeters(), i) << "TripInfo getTotalMeters differs at i = " << i;
    }
}

TEST_F(TripInfoTest, Trip_getStartPoint_test) {
    for (int p = 0; p < 0xFFFF; p++) {
        Point startPoint = Point(p, 0);
        t = TripInfo(0, 0, startPoint, Point(), 0, 0,0);
        ASSERT_TRUE(t.getStartPoint() == startPoint) << "TripInfo startPoint, expected "
                                                     << startPoint << " got "
                                                     << t.getStartPoint();
        startPoint = Point(0, p);
        t = TripInfo(0, 0, startPoint, Point(), 0, 0,0);
        ASSERT_TRUE(t.getStartPoint() == startPoint)
                                    << "TripInfo getStartPoint differs at startPoint"
                                    << startPoint;
    }
}

TEST_F(TripInfoTest, Trip_getEndPoint_test) {
    Point temp;
    Point endPoint;
    for (int p = 0; p < 0xFFFF; p++) {
        endPoint = Point(p, 0);
        t = TripInfo(0, 0, temp, endPoint, 0, 0,0);
        ASSERT_TRUE(t.getEndPoint() == endPoint) << "TripInfo endPoint, expected "
                                                 << endPoint << " got "
                                                 << t.getEndPoint();
        endPoint = Point(0, p);
        t = TripInfo(0, 0, temp, endPoint, 0, 0,0);
        ASSERT_TRUE(t.getEndPoint() == endPoint)
                                    << "TripInfo getEndPoint differs at endPoint "
                                    << endPoint;
    }
}

TEST_F(TripInfoTest, Trip_getNumOfPassengers_test) {
    Point p;
    for (unsigned int i = 0; i < 0xFFFF; i++) {
        t = TripInfo(0, 0, p, p, i, 0,0);
        ASSERT_EQ(t.getNumOfPassengers(), i) << "TripInfo getPassengers differs at i = " << i;
    }
}

TEST_F(TripInfoTest, Trip_getTariff_test) {
    Point p;
    for (unsigned int i = 0; i < 0xFFFF; i++) {
        t = TripInfo(0, 0, p, p, 0, i,0);
        ASSERT_EQ(t.getTariff(), i) << "TripInfo getTariff differs at i = " << i;
    }
}