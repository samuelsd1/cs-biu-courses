#include <gtest/gtest.h>
#include "../src/Util/Graph/BFS.h"
#include "../src/Core/Point.h"

#define POINT_MIN_VAL   -0xFFFF
#define POINT_MAX_VAL   0xFFFF
using namespace std;

namespace {
    class GraphNodeTest : public testing::Test {
    public:
        GraphNodeTest() {
        }

        static int find(const vector<GraphNode<Point> *> &nodes, const GraphNode<Point> *node) {
            for (unsigned int i = 0; i < nodes.size(); i++) {
                if (nodes[i] == node)
                    return i;
            }
            return -1;
        }
    };
}

TEST_F(GraphNodeTest, GraphNode_getData_test) {
    Point *p = new Point(1, 2);
    GraphNode<Point> g = GraphNode<Point>(p);
    EXPECT_EQ(p, g.getData());
}

TEST_F(GraphNodeTest, GraphNode_Visited_Test) {
    //test all set visited possibilities
    GraphNode<Point> g = GraphNode<Point>(new Point());
    EXPECT_FALSE(g.getVisited());
    g.setVisited(false);
    EXPECT_FALSE(g.getVisited());
    g.setVisited(true);
    EXPECT_TRUE(g.getVisited());
    g.setVisited(true);
    EXPECT_TRUE(g.getVisited());
    g.setVisited(false);
    EXPECT_FALSE(g.getVisited());
}

TEST_F(GraphNodeTest, GraphNode_Neighbors_Test) {
    //basic behaviour
    GraphNodeBase g = GraphNodeBase();
    EXPECT_TRUE(g.getNeighbors().empty());
    EXPECT_ANY_THROW(g.addNeighbor(NULL));

    //adding neighbors test
    GraphNodeBase *n1 = new GraphNodeBase();
    GraphNodeBase *n2 = new GraphNodeBase();
    GraphNodeBase *n3 = new GraphNodeBase();
    GraphNodeBase *n4 = new GraphNodeBase();
    GraphNodeBase *n5 = new GraphNodeBase();
    //add neighbor, try add it again(expect false result), and check size each time.
    //until the neighbors are full, then we can not add any neighbors.
    EXPECT_TRUE(g.addNeighbor(n1));
    EXPECT_FALSE(g.addNeighbor(n1));
    EXPECT_EQ(g.getNeighbors().size(), 1u);

    EXPECT_TRUE(g.addNeighbor(n2));
    EXPECT_FALSE(g.addNeighbor(n2));
    EXPECT_EQ(g.getNeighbors().size(), 2u);

    EXPECT_TRUE(g.addNeighbor(n3));
    EXPECT_FALSE(g.addNeighbor(n3));
    EXPECT_EQ(g.getNeighbors().size(), 3u);

    EXPECT_TRUE(g.addNeighbor(n4));
    EXPECT_FALSE(g.addNeighbor(n4));
    EXPECT_EQ(g.getNeighbors().size(), 4u);

    EXPECT_FALSE(g.addNeighbor(n5));
    vector<GraphNodeBase *> neighbors = g.getNeighbors();
    EXPECT_EQ(neighbors.size(), 4u);
    //check the indexes of the neighbors
    if (neighbors.size() == 4) {
        EXPECT_EQ(neighbors[0], n1);
        EXPECT_EQ(neighbors[1], n2);
        EXPECT_EQ(neighbors[2], n3);
        EXPECT_EQ(neighbors[3], n4);
    }
    //free all memory
    delete n1;
    delete n2;
    delete n3;
    delete n4;
    delete n5;
}

TEST_F(GraphNodeTest, GraphNode_Source_Test) {
    GraphNode<Point> *g1 = new GraphNode<Point>(new Point());
    GraphNode<Point> *g2 = new GraphNode<Point>(new Point());
    //expect they have no source
    EXPECT_TRUE(g1->getSource() == NULL);
    EXPECT_TRUE(g2->getSource() == NULL);
    //set source
    g2->setSource(g1);
    //expect source was properly setted
    EXPECT_EQ(g2->getSource(), g1);
    //free all memory
    delete g1;
    delete g2;
}