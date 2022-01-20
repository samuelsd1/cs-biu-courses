#include <gtest/gtest.h>
#include "../src/Core/Point.h"
#include "../src/Core/Map.h"

class MapTest : public testing::Test {
public:
    MapTest() {}
};

TEST_F(MapTest, Map_get_set_elements) {
    const int max_width = 10;
    const int max_height = 10;
    for (int width = -1; width <= max_width; width++) {
        for (int height = -1; height <= max_height; height++) {
            //expect throw exception for illegal values
            if (width <= 0 || height <= 0) {
                ASSERT_ANY_THROW(Map(width, height));
                continue;
            }
            //default grid is all set to empty.
            Map g = Map(width, height);
            for (int i = 0; i < width; i++) {
                for (int j = 0; j < height; j++) {
                    //check that all is empty
                    ASSERT_EQ(g.canMoveThrough(i, j), true);
                }
            }
            //set one obstacle at a time, check that all other cells arent obstacle,
            //then reset to empty cell.
            for (int i = 0; i < width; i++) {
                for (int j = 0; j < height; j++) {
                    g.setObstacleAt(i,j);
                    ASSERT_EQ(g.canMoveThrough(i, j), false);
                    for (int notI = 0; notI < width; notI++) {
                        for (int notJ = 0; notJ < height; notJ++) {
                            if (notI == i && notJ == j)
                                continue;
                            ASSERT_TRUE(g.canMoveThrough(notI, notJ)) << "Obstacle set at " << i << "," << j << "- cannot move at " << notI << "," << notJ << endl;
                        }
                    }
                    //reset back to empty
                    g.removeObstacleAt(i, j);
                }
            }
        }
    }
}

TEST_F(MapTest, Grid_FindPath1) {
    Map g = Map(3, 3);
    vector<Point> path = g.findPath(Point(0, 0), Point(2, 1));
    vector<Point> expectedPath;
    expectedPath.push_back(Point(0, 1));
    expectedPath.push_back(Point(1, 1));
    expectedPath.push_back(Point(2, 1));
    ASSERT_EQ(path.size(), expectedPath.size());
    for (unsigned int i = 0; i < path.size(); i++) {
        ASSERT_TRUE(path[i] == expectedPath[i]) << "Paths differ- expected:"
                                                << expectedPath[i] << ", reality:" << path[i];
    }
}

TEST_F(MapTest, Map_FindPath2) {
    //path with osbtacles
    Map g = Map(4, 4);
    g.setObstacleAt(2, 1);
    g.setObstacleAt(2, 2);
    g.setObstacleAt(2, 3);
    vector<Point> path = g.findPath(Point(0, 1), Point(3, 3));
    vector<Point> expectedPath;
    expectedPath.push_back(Point(1, 1));
    expectedPath.push_back(Point(1, 0));
    expectedPath.push_back(Point(2, 0));
    expectedPath.push_back(Point(3, 0));
    expectedPath.push_back(Point(3, 1));
    expectedPath.push_back(Point(3, 2));
    expectedPath.push_back(Point(3, 3));
    //expect same size
    ASSERT_EQ(path.size(), expectedPath.size());
    //check each point
    for (unsigned int i = 0; i < path.size(); i++) {
        ASSERT_TRUE(path[i] == expectedPath[i]) << "Paths differ- expected:" << expectedPath[i]
                                                << ", reality:" << path[i];
    }
}
