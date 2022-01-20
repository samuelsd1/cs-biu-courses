#ifndef AP1_EX1_BFS_H
#define AP1_EX1_BFS_H

#include <queue>
#include "GraphAlgorithm.h"
#include "GraphNode.h"
#include <cstdio>

using namespace std;

/**
 * BFS algorithm implementation modified to fit to the current project.
 * used to find shortest route in the grid.
 */
class BFS : public GraphAlgorithm {
public:
    virtual ~BFS();
protected:
    /**
    * Bfs algorithm, implementation copied from wikipedia's hebrew page and modified
    * to fit to this project.
    * @param src - pointer to the source node.
    * @param dest - pointer to the desination node.
    * @return vector of nodes, which holds the path from src to dest (including dest, excluding src).
     * if no path found, an empty vector returned.
     */
    std::vector<GraphNodeBase *> _findRoute(GraphNodeBase *src, GraphNodeBase *dest);
};


#endif //AP1_EX1_BFS_H
