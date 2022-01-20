#include <cstdio>
#include <gtest/gtest.h>
#include "../src/Util/Graph/BFS.h"
#include "../src/Core/Point.h"

#define POINT_MIN_VAL   -0xFFFF
#define POINT_MAX_VAL   0xFFFF
using namespace std;

namespace {
    class BfsTest : public testing::Test {
    public:
        BfsTest() {
            bfs = new BFS();
        }
        ~BfsTest(){
            delete bfs;
        }

    protected:
        GraphAlgorithm *bfs;
    };
}

TEST_F(BfsTest, TestBFS_NoPath) {
    GraphNode<Point> *g1 = new GraphNode<Point>(new Point());
    GraphNode<Point> *g2 = new GraphNode<Point>(new Point());
    vector<GraphNode<Point> *> expectedEmpty = bfs->findRoute(g1, g2);
    delete g1;
    delete g2;
    ASSERT_TRUE(expectedEmpty.empty());

}

TEST_F(BfsTest, TestBfs_3_4_ClearPath) {
    const int width = 3;
    const int height = 4;
    GraphNode<Point> *graph[width][height];
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            graph[i][j] = new GraphNode<Point>(new Point(i, j));
        }
    }
    //set neighbors
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            //if cell is null (obstacle)
            if (graph[i][j] == NULL)
                //continue, cannot set neighbors to nothing.
                continue;
            //left neighbor
            if (i > 0)
                if (graph[i - 1][j] != NULL)
                    graph[i][j]->addNeighbor(graph[i - 1][j]);
            //top neighbor
            if (j + 1 < height)
                if (graph[i][j + 1] != NULL)
                    graph[i][j]->addNeighbor(graph[i][j + 1]);
            //right neighbor
            if (i + 1 < width)
                if (graph[i + 1][j] != NULL)
                    graph[i][j]->addNeighbor(graph[i + 1][j]);
            //bottom neighbor
            if (j > 0)
                if (graph[i][j - 1] != NULL)
                    graph[i][j]->addNeighbor(graph[i][j - 1]);
        }
    }

    //path 1:
    vector<GraphNode<Point> *> route = bfs->findRoute(graph[0][0], graph[2][1]);
    //fill expected path
    vector<GraphNode<Point> *> expectedPath;
    expectedPath.push_back(graph[0][1]);
    expectedPath.push_back(graph[1][1]);
    expectedPath.push_back(graph[2][1]);

    //expect same size of paths
    EXPECT_EQ(route.size(), expectedPath.size()) << "Difference at Bfs Clear Path 1 Test";
    if (route.size() == expectedPath.size()) {
        //check every point
        for (unsigned int i = 0; i < route.size(); i++) {
            EXPECT_EQ(route[i], expectedPath[i]);
        }
    }
    //clear the vectors
    expectedPath.clear();
    route.clear();
    //reset the visited and source mark.
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            graph[i][j]->setSource(NULL);
            graph[i][j]->setVisited(false);
        }
    }

    //path 2:
    route = bfs->findRoute(graph[2][0], graph[1][3]);
    //fill expected oath
    expectedPath.push_back(graph[1][0]);
    expectedPath.push_back(graph[1][1]);
    expectedPath.push_back(graph[1][2]);
    expectedPath.push_back(graph[1][3]);

    //expect same size of paths
    EXPECT_EQ(route.size(), expectedPath.size()) << "Difference at Bfs Clear Path 2 Test";;
    if (route.size() == expectedPath.size()) {
        //check every point
        for (unsigned int i = 0; i < route.size(); i++) {
            EXPECT_EQ(route[i], expectedPath[i]);
        }
    }
    //clear the path vectors
    expectedPath.clear();
    route.clear();
    //reset the visited and source mark.
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            graph[i][j]->setSource(NULL);
            graph[i][j]->setVisited(false);
        }
    }

    //path 3:
    route = bfs->findRoute(graph[0][3], graph[2][0]);
    //fill expected oath
    expectedPath.push_back(graph[1][3]);
    expectedPath.push_back(graph[2][3]);
    expectedPath.push_back(graph[2][2]);
    expectedPath.push_back(graph[2][1]);
    expectedPath.push_back(graph[2][0]);

    //expect same size of paths
    EXPECT_EQ(route.size(), expectedPath.size()) << "Difference at Bfs Clear Path 3 Test";;
    if (route.size() == expectedPath.size()) {
        //check every point
        for (unsigned int i = 0; i < route.size(); i++) {
            EXPECT_EQ(route[i], expectedPath[i]);
        }
    }
    //clear the path vectors
    expectedPath.clear();
    route.clear();
    //reset the visited and source mark.
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            graph[i][j]->setSource(NULL);
            graph[i][j]->setVisited(false);
        }
    }

    //path 4:
    route = bfs->findRoute(graph[2][3], graph[0][0]);
    //fill expected oath
    expectedPath.push_back(graph[1][3]);
    expectedPath.push_back(graph[0][3]);
    expectedPath.push_back(graph[0][2]);
    expectedPath.push_back(graph[0][1]);
    expectedPath.push_back(graph[0][0]);

    //expect same size of paths
    EXPECT_EQ(route.size(), expectedPath.size()) << "Difference at Bfs Clear Path 4 Test";;
    if (route.size() == expectedPath.size()) {
        //check every point
        for (unsigned int i = 0; i < route.size(); i++) {
            EXPECT_EQ(route[i], expectedPath[i]);
        }
    }
    //clear the path vectors
    expectedPath.clear();
    route.clear();

    //delete the graph
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            delete graph[i][j];
        }
    }
}

TEST_F(BfsTest, BFS_Obstacle_Path) {
    const int width = 3;
    const int height = 3;
    //create the graph
    GraphNode<Point> *graph[width][height];
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            //set obstacles at (1,1) (1,2)
            if (i == 1 && j != 0) {
                graph[i][j] = NULL;
                continue;
            }
            graph[i][j] = new GraphNode<Point>(new Point(i, j));
        }
    }

    //set neighbors
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            //if cell is null (obstacle)
            if (graph[i][j] == NULL)
                //continue, cannot set neighbors to nothing.
                continue;
            //left neighbor
            if (i > 0)
                if (graph[i - 1][j] != NULL)
                    graph[i][j]->addNeighbor(graph[i - 1][j]);
            //top neighbor
            if (j + 1 < height)
                if (graph[i][j + 1] != NULL)
                    graph[i][j]->addNeighbor(graph[i][j + 1]);
            //right neighbor
            if (i + 1 < width)
                if (graph[i + 1][j] != NULL)
                    graph[i][j]->addNeighbor(graph[i + 1][j]);
            //bottom neighbor
            if (j > 0)
                if (graph[i][j - 1] != NULL)
                    graph[i][j]->addNeighbor(graph[i][j - 1]);
        }
    }

    //path
    vector<GraphNode<Point> *> route = bfs->findRoute(graph[0][2], graph[2][2]);
    vector<GraphNode<Point> *> expectedPath;
    //fill expected path
    expectedPath.push_back(graph[0][1]);
    expectedPath.push_back(graph[0][0]);
    expectedPath.push_back(graph[1][0]);
    expectedPath.push_back(graph[2][0]);
    expectedPath.push_back(graph[2][1]);
    expectedPath.push_back(graph[2][2]);

    //expect same size of paths
    EXPECT_EQ(route.size(), expectedPath.size()) << "Difference at Bfs Obstacle Path Test";;
    if (route.size() == expectedPath.size()) {
        //check every point
        for (unsigned int i = 0; i < route.size(); i++) {
            EXPECT_EQ(route[i], expectedPath[i]);
        }
    }
    //clear the path vectors
    expectedPath.clear();
    route.clear();

    //delete the graph
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            delete graph[i][j];
        }
    }
}
