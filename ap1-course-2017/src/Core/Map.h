#ifndef AP1_EX1_GRID_H
#define AP1_EX1_GRID_H

#include <vector>
#include "Point.h"
#include "../Util/Graph/GraphAlgorithm.h"
#include "../Util/easylogging++.h"

using namespace std;

class Map {
public:
    /**
     * Grid constructor
     * @param width - the width of the grid
     * @param height - the height of the grid
     */
    Map(const int & width,const int & height);
    virtual ~Map();

    unsigned int getWidth() const;
    unsigned int getHeight() const;

    bool isLegalIndex(const int &x, const int &y) const;
    bool isLegalIndex(const Point &p) const;

    bool canMoveThrough(const int &x, const int &y) const;
    bool canMoveThrough(const Point &p) const;
    void setObstacleAt(const int &x, const int &y);
    void removeObstacleAt(const int &x, const int &y);
    void setObstacles(const vector<Point> & obstacles);

    /**
     * Function finds shortest path between src and dest points.
     * if no path found or illegal source/destination points given, the function
     * will return empty vector of points.
     * @param src - source point of the path
     * @param dest - destination point for the path
     * @return vector of points which contains the path from src point to the dest point.
     *         if no path found or illegal source/destination points given, it returns empty vector
     */
    vector<Point> findPath(const Point & src,const Point & dest) const;

    /**
     * function prints the grid in a nice way.
     * used to debug mostly, but it is pretty nice.
     */
    void print() const;
private:
    unsigned int _width, _height;
    bool ** grid;
    GraphAlgorithm *algorithm;
};
#endif //AP1_EX1_GRID_H
