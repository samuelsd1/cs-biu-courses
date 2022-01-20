#ifndef AP1_EX1_GRAPHNODE_H
#define AP1_EX1_GRAPHNODE_H

#define MAX_NEIGHBORS   4
#include <vector>
#include <cstdio>

class GraphNodeBase{
public:
    GraphNodeBase(){
        _src = NULL;
        _visited = false;
    }

    virtual ~GraphNodeBase(){
        _src = NULL;
        _visited = false;
    }

    /**
    * node's 'visited' property getter.
    * @return whether the node was visited or no.
    */
    bool getVisited() const{
        return _visited;
    }

    /**
    * node's 'visited' property setter
    * @param visited - represents whether the node was visited before.
    */
    void setVisited(bool visited){
        _visited = visited;
    }

    /**
    * node's source getter
    * @return pointer to the source of the node.
    */
    GraphNodeBase *getSource() const{
        return _src;
    }

    /**
    * node's source setter
    * @param src - pointer0 to the source of the node.
    */
    void setSource(GraphNodeBase *src){
        _src = src;
    }

    /**
     * neighbors getter
     * @return vector of <GraphNodeBase*> contains the neighbors of the node
     */
    std::vector<GraphNodeBase *> getNeighbors() const{
        return neighbors;
    }

    /**
     * function gets a neighbor and adds it to the current node if there is enough space.
     * there are enough space for 4 neighbors.
     * @param neighbor - another node, which will be a neighbor for the current node.
     * @return bool representing the success of the operation
     */
    bool addNeighbor(GraphNodeBase *neighbor){
        if (neighbor == NULL)
            throw "GraphNode addNeighbor received NULL neighbor.";
        //check if there aren't too much of neighbors.
        if (neighbors.size() >= MAX_NEIGHBORS)
            return false;
        //check that the same neighbor wasnt inserted twice
        for(unsigned int i=0;i<neighbors.size();i++){
            if(neighbor == neighbors[i])
                return false;
        }
        neighbors.push_back(neighbor);
        return true;
    }

private:
    bool _visited;
    std::vector<GraphNodeBase *> neighbors;
    GraphNodeBase *_src;
};

/**
 * GraphNode is a single node in a graph, and is used in the BFS algorithm.
 */
template <typename DataType>
class GraphNode : public GraphNodeBase{
public:
    /**
    * GraphNode constructor
    * @param p - a point that the node saves
    */
    GraphNode(DataType *p){
        data = p;
    }

    /**
    * GraphNode Destructor
    */
    virtual ~GraphNode(){
        delete data;
    }

    /**
    * node's data getter, gets the data that the node contains.
    * @return the data that the node contains.
    */
    DataType *getData() const {
        return data;
    }
private:
    DataType *data;
};


#endif //AP1_EX1_GRAPHNODE_H
