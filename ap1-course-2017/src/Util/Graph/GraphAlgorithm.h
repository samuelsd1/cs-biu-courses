#ifndef AP1_GRAPHALGORITHM_H
#define AP1_GRAPHALGORITHM_H

#include <vector>
#include "GraphNode.h"

class GraphAlgorithm {
public:
    virtual ~GraphAlgorithm() {}
    /**
     * function gets source GraphNode and destination GraphNode and their type(in the template),
     * and returns vector of the shortest path between src and dest (excluding src, including dest).
     * if no path found, it returns an empty vector
     * @template - Type of the GraphNodes
     * @param src - the source node
     * @param dest - the destination node
     */
    template <typename NodeType>
    std::vector<GraphNode<NodeType> *> findRoute(GraphNode<NodeType> *src, GraphNode<NodeType> *dest) {
        //this is a nice trick to get a generic graphNode and generic algorithm, took long to find it.
        //the algorithm searches on the GraphNodeBase, and the graphNode type doesn't really matter to it.
        //so I did an upcast to graphNodeBase for the algorithm, and then down casting back to the typed graph node.
        //the down casting considered safe - because the type does not change during the search.
        // Casting could not be performed on the whole vector, So here casting is performed Node by Node.
        //get path vector of the route, of type GraphNodeBase*
        std::vector<GraphNodeBase *> v = _findRoute(src, dest);
        std::vector<GraphNode<NodeType>*> typedVector;
        for(unsigned int i=0;i<v.size();i++){
            //cast back each node to its original type. it is safe casting - because the upcasting was
            //done here as well.
            typedVector.push_back((GraphNode<NodeType> *)v[i]);
        }
        return typedVector;
    }

protected:
    virtual std::vector<GraphNodeBase *> _findRoute(GraphNodeBase *src, GraphNodeBase *dest) = 0;
};


#endif //AP1_GRAPHALGORITHM_H
