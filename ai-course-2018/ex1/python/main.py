import sys
import numpy as np
from Queue import PriorityQueue

class MyPriorityQueue(PriorityQueue):
    def __init__(self):
        PriorityQueue.__init__(self)
        self.counter = 0

    def put(self, item, priority):
        PriorityQueue.put(self, (priority, self.counter, item))
        self.counter += 1

    def get(self, *args, **kwargs):
        _, _, item = PriorityQueue.get(self, *args, **kwargs)
        return item

_values = {'S':1, 'G':1,'R':1, 'D':3, 'H':10, 'W':1e10}
S2I = {c: i for i, c in enumerate(sorted(_values))}
I2S = {v: k for k, v in S2I.iteritems()}

def parse_input(input_fname):
    lines = []
    with open(input_fname, 'r') as file:
        lines = [line for line in file]

    alg_type = lines[0]
    board_size = int(lines[1])
    lines = lines[2:]

    map = np.zeros((board_size,board_size))

    for i in range(board_size):
        for j in range(board_size):
            map[i,j] = S2I[lines[i][j]]

    return alg_type,map

class Searchable:

    def neighbors(self, curr):
        raise NotImplementedError()

    def start(self):
        raise NotImplementedError()

    def goal(self):
        raise NotImplementedError()

    def cost(self, src, target):
        raise NotImplementedError()

class GameMap(Searchable):

    def __init__(self,map):
        self.map = map
        self.start= None
        self.goal = None

        rows = map.shape[0]
        cols = map.shape[1]
        for row in xrange(rows):
            for col in xrange(cols):
                if self.start == None and map[row, col] == S2I['S']:
                    self.start = (row, col)

                if self.goal == None and map[row,col] == S2I['G']:
                    self.goal = (row,col)

    def neighbors(self, curr):
        c_row,c_col = curr
        tag = -np.ones((3,3))

        def legal_index(r,c):
            n = self.map.shape[0]
            return r >= 0 and r < n and c >= 0 and c < n

        for di in range(0,3):
            for dj in range(0,3):
                row,col = (c_row+di-1,c_col+dj-1)

                if not legal_index(row,col):
                    continue

                if self.map[row,col] == S2I['W']:
                    continue

                tag[di,dj] = self.map[row,col]

        # center (stay at your position)
        tag[1,1] = -1

        # up
        if tag[0,1] == -1:
            tag[0,0] = tag[0,2] = -1
        # down
        if tag[2,1] == -1:
            tag[2,0] = tag[2,2] = -1
        # left
        if tag[1,0] == -1:
            tag[0,0] = tag[2,0] = -1
        # right
        if tag[1,2] == -1:
            tag[0,2] = tag[2,2] = -1

        possible_states = []



        indices_to_iterate = {(1,2),(2,2),(2,1),(2,0),(1,0),(0,0),(0,1),(0,2)}

        for index in indices_to_iterate:
            di,dj = index
            if tag[di,dj] == -1: continue
            row = c_row+di-1
            col = c_col+dj-1

            state = (row,col)
            #state.came_from = curr
            #state.cost = _values[I2S[self.map[row,col]]]
            possible_states.append(state)

        return possible_states

    def start(self):
        return self.start

    def goal(self):
        return self.goal

    def cost(self, src, target):
        t0s0 = target[0]-src[0]
        t1s1 = target[1]-src[1]
        if abs(t0s0) + abs(t1s1) > 2:
            return 1e10
        return _values[I2S[self.map[src]]]

class Searcher:

    def search(self, searchable):
        raise NotImplementedError()

    def getEvaluatedNodes(self):
        raise NotImplementedError()


class AStarSearch(Searcher):

    def __init__(self, heuristic):
        self.heuristic = heuristic

    def search(self,searchable):
        frontier = MyPriorityQueue()

        goal = searchable.goal
        start = searchable.start

        frontier.put(start, 0)

        came_from = {}
        cost_so_far = {}
        came_from[start] = None
        cost_so_far[start] = 0


        while not frontier.empty():
            current = frontier.get()

            print 'Current from frontier:',current
            if current == goal:
                break

            for next in searchable.neighbors(current):
                if next == current:
                    continue

                if next in frontier.queue:
                    continue

                new_cost = cost_so_far[current] + searchable.cost(current, next)
                print '\tNext is %s, and its new cost is %s'%(next,new_cost)
                #if next not in cost_so_far or new_cost < cost_so_far[next]:    # with close list
                if new_cost < cost_so_far.get(next,1e10):
                    cost_so_far[next] = new_cost
                    priority = new_cost + self.heuristic(goal, next)
                    frontier.put(next, priority)
                    print '\t\t inserting',next,'with priority',priority
                    came_from[next] = current

        return came_from

class IDSSearcher(Searcher):

    def __init__(self, max_depth):
        self.max_depth = max_depth


    def search(self, searchable):
        # A function to perform a Depth-Limited search

        def DLS_Visit(parent,src, target, maxDepth):
            # if found
            if src == target:
                print 'Found'
                return True

            # If reached the maximum depth, stop recursing.
            if maxDepth <= 0: return False

            # Recur for all the vertices adjacent to this vertex
            for v in searchable.neighbors(src):
                if v not in parent:
                    parent[v] = src
                    found = DLS_Visit(parent,v, target, maxDepth - 1)
                    if found:
                        return True
            return False

        # IDDFS to search if target is reachable from v.
        # It uses recursive DLS()
        def IDDFS(src, target, maxDepth):
            # Repeatedly depth-limit search till the
            # maximum depth
            for depth in range(maxDepth):
                parent = {}
                found = DLS_Visit(parent, src, target, depth)
                if found:
                    return parent
            return {}

        return IDDFS(searchable.start, searchable.goal, self.max_depth)

    def getEvaluatedNodes(self):
        raise NotImplementedError()

def backtrace_solution(came_from, goal):
    target = goal
    src = came_from[goal]
    path = []

    deltas = {(0,1):'R',(1,1):'RD',(1,0):'D',(1,-1):'LD',(0,-1):'L',(-1,-1):'LU',(-1,0):'U',(-1,1):'RU'}
    while src != None:
        delta = (target[0]-src[0],target[1]-src[1])
        path.append(deltas[delta])

        target = src
        src = came_from.get(src,None)

    path.reverse()
    return path

def main(input_fname):
    alg_type,map = parse_input(input_fname)

    gmap = GameMap(map)
    print 'Map:'
    print map
    print '---'

    depth = map.shape[0] ** 2
	
    #print gmap.neighbors((2,1))
    #exit()

    def heuristic(one,two):
        x = abs(two[0]-one[0])
        y = abs(two[1]-one[1])
        return np.sqrt(x*x + y*y)



    searcher = IDSSearcher(30)

    came_from = searcher.search(gmap)
    #print 'Came From:'
    #print came_from

    path = backtrace_solution(came_from,gmap.goal)
    print path


if __name__ == '__main__':
    if len(sys.argv) <= 1:
        print 'Usage: main.py <input_filename>'
        exit()

    main(sys.argv[1])