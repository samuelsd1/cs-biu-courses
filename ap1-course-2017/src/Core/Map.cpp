#include <iostream>
#include "Map.h"
#include "../Util/Graph/BFS.h"

/**
* Grid constructor
* @param width - the width of the grid
* @param height - the height of the grid
*/
Map::Map(const int &width, const int &height) {
    if (width < 1 || height < 1)
        throw "Map Illegal size at constructor.";
    //save the width and height
    _width = (unsigned int) width;
    _height = (unsigned int) height;

    //allocate memory for the grid
    grid = new bool *[width];

    //initialize with clear path
    for (int i = 0; i < width; i++) {
        grid[i] = new bool[height];
        for (int j = 0; j < height; j++) {
            grid[i][j] = true;
        }
    }

    algorithm = new BFS();

    //print();
}

/**
 * Grid Destructor
 */
Map::~Map() {
    if (grid != NULL) {
        //free the grid
        for (unsigned int i = 0; i < _width; i++) {
            delete[] grid[i];
        }
        delete[] grid;
        grid = NULL;
    }

    if (algorithm != NULL) {
        delete algorithm;
        algorithm = NULL;
    }
}

/**
 * width getter
 */
unsigned int Map::getWidth() const {
    return _width;
}

/**
 * height getter
 */
unsigned int Map::getHeight() const {
    return _height;
}

bool Map::isLegalIndex(const int &x, const int &y) const {
    if (x < 0 || x >= (int) _width || y < 0 || y >= (int) _height)
        return false;
    return true;
}

bool Map::isLegalIndex(const Point &p) const {
    return isLegalIndex(p.getX(), p.getY());
}

/**
 * gets x and y coord and returns whether its empty (means there is no obstacle there)
 */
bool Map::canMoveThrough(const int &x, const int &y) const {
    if (!isLegalIndex(x, y))
        return false;
    return grid[x][y];
}

/**
 * gets x and y coord and returns whether its empty (means there is no obstacle there)
 */
bool Map::canMoveThrough(const Point &location) const {
    return canMoveThrough(location.getX(), location.getY());
}

/**
 * sets an obstacle in the given position
 */
void Map::setObstacleAt(const int &x, const int &y) {
    if (!isLegalIndex(x, y))
        throw "Map::setObstacleAt - received index out of bounds";
    grid[x][y] = false;
}

/**
 * removes an obstacle from the given position
 */
void Map::removeObstacleAt(const int &x, const int &y) {
    if (!isLegalIndex(x, y))
        throw "Map::removeObstacleAt - index out of bounds received";
    grid[x][y] = true;
}

/**
 * sets obstacles on the grid
 * @param obstacles vector of points which represent obstacles on the grid.
 */
void Map::setObstacles(const vector<Point> &obstacles) {
    for (unsigned int i = 0; i < obstacles.size(); i++) {
        Point obstacle = obstacles[i];
        //if the obstacle is out of bounds, do not add it
        if (!isLegalIndex(obstacle))
            continue;
        //obstacle is legal, add it to the grid.
        grid[obstacle.getX()][obstacle.getY()] = false;
    }
}

/**
* Function finds shortest path between src and dest points.
* if no path found or illegal source/destination points given, the function
* will return empty vector of points.
* @param src - source point of the path
* @param dest - destination point for the path
* @return vector of points which contains the path from src point to the dest point.
*         if no path found or illegal source/destination points given, it returns empty vector.
*/
vector<Point> Map::findPath(const Point &src, const Point &dest) const {
    //create 2d array of graphNode*, assign neighbors to each node and then search
    //using the bfs algorithm.
    bool canSearch = true;
    //prevent out of bounds access
    if (!isLegalIndex(src))
        canSearch = false;
    if (!isLegalIndex(dest))
        canSearch = false;
    //if indexes are right,
    if (canSearch) {
        //disable searching from and to obstacles.
        if (!grid[src.getX()][src.getY()] || !grid[dest.getX()][dest.getY()])
            canSearch = false;
    }
    vector<Point> path;
    //if cannot search, return empty path.
    if (!canSearch){
        LERROR << "Cannot calculate the path: bad arguments received.";
        return path;
    }

    PDEBUG << "Starting to calculate the path from " << src << " to " << dest << ".";
    clock_t begin = clock();
    //allocate space for graph
    GraphNode<Point> ***graph = new GraphNode<Point> **[_width];
    for (unsigned int i = 0; i < _width; i++) {
        graph[i] = new GraphNode<Point> *[_height];
    }

    //initialize with values
    for (unsigned int i = 0; i < _width; i++) {
        for (unsigned int j = 0; j < _height; j++) {
            //if there is obstacle, set null.
            if (!grid[i][j])
                graph[i][j] = NULL;
            else
                graph[i][j] = new GraphNode<Point>(new Point(i, j));
        }
    }
    //set neighbors
    for (unsigned int i = 0; i < _width; i++) {
        for (unsigned int j = 0; j < _height; j++) {
            //if cell is null (obstacle)
            if (graph[i][j] == NULL)
                //continue, cannot set neighbors to nothing.
                continue;

            //left neighbor
            if (i > 0)
                if (graph[i - 1][j] != NULL)
                    graph[i][j]->addNeighbor(graph[i - 1][j]);

            //top neighbor
            if (j + 1 < _height)
                if (graph[i][j + 1] != NULL)
                    graph[i][j]->addNeighbor(graph[i][j + 1]);

            //right neighbor
            if (i + 1 < _width)
                if (graph[i + 1][j] != NULL)
                    graph[i][j]->addNeighbor(graph[i + 1][j]);

            //bottom neighbor
            if (j > 0)
                if (graph[i][j - 1] != NULL)
                    graph[i][j]->addNeighbor(graph[i][j - 1]);

        }
    }
    //search the shortest path using the given algorithm
    vector<GraphNode<Point> *> route = algorithm->findRoute(
            graph[src.getX()][src.getY()],
            graph[dest.getX()][dest.getY()]);
    //fill the path vector with the points received from the bfs algorithm
    for (unsigned int i = 0; i < route.size(); i++) {
        Point p = *(route[i]->getData());
        path.push_back(p);
    }

    //dispose the graph
    for (unsigned int i = 0; i < _width; i++) {
        for (unsigned int j = 0; j < _height; j++) {
            if (graph[i][j] != NULL)
                //delete each Cell - GraphNode*
                delete graph[i][j];
        }
        //delete each column - height
        delete[] graph[i];
    }
    //delete the row - width
    delete[] graph;
    clock_t end = clock();
    //calculate how math time finding path has been taken.
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    PDEBUG << "Calculated the path from " << src << " to " << dest << ". path size: " << path.size()
          << ", elapsed time: " << elapsed_secs << " seconds.";
    //return the path
    return path;
}

/**
 * function prints the grid in a nice way.
 * used to debug mostly, but it is pretty nice.
 */
void Map::print() const {
    cout << "Map: " << endl;
    cout << " _";
    for (unsigned int i = 0; i < _width; i++) {
        cout << "__";
    }
    cout << endl;
    for (int j = _height - 1; j >= 0; j--) {
        cout << "|";
        for (unsigned int i = 0; i < _width; i++) {
            cout << " ";
            if (!grid[i][j]) {
                cout << "X";
            } else {
                cout << "o";
            }
        }
        cout << " |" << endl;
    }
    cout << " -";
    for (unsigned int i = 0; i < _width; i++) {
        cout << "--";
    }
    cout << endl;
}
