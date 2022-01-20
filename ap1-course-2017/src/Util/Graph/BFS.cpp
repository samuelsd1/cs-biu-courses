#include "BFS.h"

/**
 * Default Dtor
 */
BFS::~BFS() {}

/**
* Bfs algorithm, implementation copied from wikipedia's hebrew page and modified
* to fit to this project.
* @param src - pointer to the source node.
* @param dest - pointer to the desination node.
* @return vector of nodes, which holds the path from src to dest,source on index 0.
 * if no path found, an empty vector returned.
 */
std::vector<GraphNodeBase *>  BFS::_findRoute(GraphNodeBase *src, GraphNodeBase *dest) {
    // there is nothing much to explain here, its just implementation of the pseudo code
    // from the hebrew page of the BFS algorithm

    if (src == NULL || dest == NULL)
        throw "Null graph Node received. aborting BFS search.";

    vector<GraphNodeBase *> path;
    queue<GraphNodeBase *> q;
    q.push(src);

    while (!q.empty()) {
        GraphNodeBase *currentNode = q.front();
        q.pop();
        currentNode->setVisited(true);
        if (currentNode == dest) {
            //found the path
            GraphNodeBase *trace = currentNode;
            while (trace != src) {
                path.insert(path.begin(), trace);
                trace = trace->getSource();
            }
            break;
        }
        vector<GraphNodeBase *> neighbors = currentNode->getNeighbors();
        for (unsigned int i = 0; i < neighbors.size(); i++) {
            if (!neighbors[i]->getVisited()) {
                neighbors[i]->setSource(currentNode); // for path tracing
                neighbors[i]->setVisited(true); // visited to avoid loops
                q.push(neighbors[i]);
            }
        }
    }
    return path;
}
