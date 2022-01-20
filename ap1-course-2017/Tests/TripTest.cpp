#include <gtest/gtest.h>
#include "../src/Core/Point.h"
#include "../src/Core/Trip/TripInfo.h"
#include "../src/Core/Trip/Trip.h"

using namespace std;

class TripTest : public testing::Test {
public:
    TripTest() {}

protected:
    TripInfo t;
};

TEST_F(TripTest, Trip_Constructor_Throws_Test) {
    //TripInfo* ti =
    Trip* t = NULL;
    deque<Point> path;
    path.push_back(Point(1,1));
    path.push_back(Point(2,2));
    path.push_back(Point(3,3));
    EXPECT_ANY_THROW(t = new Trip(NULL, path));
    //in case that test failed, still need to delete object
    if(t != NULL){
        delete t;
        t = NULL;
    }

    EXPECT_ANY_THROW(t = new Trip(new TripInfo(0, 0, Point(0,0), Point(3,4), 2, 2.5,0), path));
    //in case that test failed, still need to delete object
    if(t != NULL){
        delete t;
        t = NULL;
    }

    EXPECT_NO_THROW(t = new Trip(new TripInfo(0, 0, Point(0,0), Point(3,3), 2, 2.5,0), path));
    //in case that test failed, still need to delete object correctly
    if(t != NULL){
        delete t;
        t = NULL;
    }
}

TEST_F(TripTest, Trip_Move_Single_Step_Test) {
    Trip* t = NULL;
    Point expectedPoint;
    deque<Point> path;
    path.push_back(Point(1,1));
    path.push_back(Point(2,2));
    path.push_back(Point(3,3));
    //create the tested trip
    t = new Trip(new TripInfo(0, 0, Point(0,0), Point(3,3), 2, 2.5,0), path);
    while(!path.empty()){
        expectedPoint = path.front();
        path.pop_front();
        Point realPoint = t->moveSteps(1);
        EXPECT_TRUE(realPoint == expectedPoint) << "Current point expected to be " << expectedPoint << ", instead found " << realPoint << endl;
    }
    //expect the trip to be finished
    EXPECT_TRUE(t->finished());

    //expect not to move after trip has ended, no matter how much steps we request.
    expectedPoint = Point(3,3);
    for(int i=-10;i<10;i++){
        EXPECT_TRUE(expectedPoint == t->moveSteps(i));
    }
    delete t;
}

TEST_F(TripTest, Trip_Move_Steps_Test) {
    Trip* t = NULL;
    Point expectedPoint;
    deque<Point> path;
    path.push_back(Point(1,1));
    path.push_back(Point(2,2));
    path.push_back(Point(3,3));
    //create the tested trip
    t = new Trip(new TripInfo(0, 0, Point(0,0), Point(3,3), 2, 2.5,0), path);
    //expect good updated location
    EXPECT_TRUE(t->moveSteps(2) == Point(2,2));
    //expect good updated location
    EXPECT_TRUE(t->moveSteps(2) == Point(3,3));
    //expect the trip to be finished
    EXPECT_TRUE(t->finished());

    //expect not to move after trip has ended, no matter how much steps we request.
    expectedPoint = Point(3,3);
    for(int i=-10;i<10;i++){
        EXPECT_TRUE(expectedPoint == t->moveSteps(i));
    }
    delete t;
}