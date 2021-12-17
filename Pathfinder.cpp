#include <iostream>
#include <fstream>
#include <limits>
#include <vector>
#include <utility>
#include <queue>

using namespace std;

typedef pair<int64_t, int64_t> iPair;

struct Node
{
    Node(const int64_t _cost, const int64_t _value, const iPair _cord): cost {_cost}, cord {_cord},  value {_value}{}
    int64_t value; 
    int64_t cost;
    iPair cord;
    iPair prev = {-1, -1};
    bool visited = false;
    bool marked = false;

};

struct Comparator
{
    int operator()(const Node& n1, const Node& n2)
    {
        return n1.cost > n2.cost;
    }
};

void print_grid(const vector<vector<Node>>& grid)
{
    for(auto& vec: grid)
    {
        for(auto& node: vec)
        {
            if(node.marked)
                //cout << YELLOW << node.value << RESET;
                cout << "🟥";
            else if(node.value == 0)
                //cout << node.value;
                cout << "⬛️";
            else
                cout << "⬜️";
        }
        cout << '\n';
    }
}

/**
 * @brief apply djikstar algorithm to given grid.
 * calculate all paths in grid. Mark shortest path from start to end point and return length of said path
 * 
 * @param grid grid containing Nodes with 1 and 0 values. 1 can be visited while 2 are walls
 * @param start start point of algorithm. Defaults to y = 0, x = 1 as the maze generator starts at said location
 * @param end end point of algorithm. Defaults to y = height - 1 and x = width - 2, as the maze generator inserts end at said location
 * @return int64_t length of shortest path from start to end
 */
int64_t dijkstra(vector<vector<Node>>& grid, const iPair start = {1, 1}, iPair end = {-1, -1})
{   
    //get size of grid
    const int64_t height = grid.size(), width = grid.front().size();

    //default end
    if(end == iPair(-1 , -1))
        end = iPair(height - 2, width - 2);

    //get start and end points
    auto [y_s, x_s] = start;
    auto [y_e, x_e] = end;

    //throw exception if start or end are not in range
    if(y_s < 0 || y_s >= height || x_s < 0 || x_s >= width)
        throw out_of_range("Start Point not on grid");
    if(y_e < 0 || y_e >= height || x_e < 0 || x_e >= width)
        throw out_of_range("End Point not on grid");

    //priority queue (min-heap)
    priority_queue<Node, vector<Node>, Comparator> to_visit;
    //array containing coordinates of potential neighbors
    const int64_t neighbors[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    //mark start
    grid[y_s][x_s].marked = true;
    //put start point in queue
    to_visit.emplace(grid[y_s][x_s]);

    //runs until queue is empty
    while(!to_visit.empty())
    {
        //get values from element with smallest cost in queue
        const int64_t cost = to_visit.top().cost, y = to_visit.top().cord.first, x = to_visit.top().cord.second;
        //remove element from queue
        to_visit.pop();

        //for every possible neighbor of current node
        for(auto n: neighbors) 
        {   
            //calculate coordinates of neighbor
            const int64_t new_y = n[0] + y, new_x = n[1] + x;
            //skip neighbor if node has been marked or if coordinates are out of bound
            if (new_y < 0 || new_y >= height || new_x < 0 || new_x >= width || 
                grid[new_y][new_x].visited == true || grid[new_y][new_x].value == 0)
                continue;

            //set cost of neighbor to current cost plus value of neighbor
            grid[new_y][new_x].cost = cost + (grid[new_y][new_x].value);
            
            //mark neighbor as visited
            grid[new_y][new_x].visited = true;
            grid[new_y][new_x].prev = iPair(y, x);
            //push neighbor into queue
            to_visit.push(grid[new_y][new_x]);
        }
    }

    //start at end node, moving to it's prev location, marking eveything on its way
    Node* iter = &grid[y_e][x_e];
    while (iter->cord != start)
    {   
        //if pair is unchanged, no path has been found
        if(iter->prev == iPair(-1, -1))
            return -1;
        //get cords of prev
        auto [y, x] = iter->prev;
        iter->marked = true;
        //move pointer to prev location
        iter = &grid[y][x];
    }
        
    //return stored in end node
    return grid[y_e][x_e].cost;
}


int main(int argc, char* argv[])
{   
    ifstream file;
    if(argc >= 2)
        file.open(argv[1]);
    else
        file.open("maze.txt");

    string in;
    int64_t temp_value, height, width;
    vector<vector<Node>> grid;

    //read file and create grid for Part 1
    for(int64_t i = 0; getline(file, in); i++)
    {
        grid.push_back(vector<Node>());
        for(int64_t j = 0; j < in.size(); j++)
        {
            grid.back().push_back(Node(0, in[j] - '0', iPair(i, j)));
        }
    }

    dijkstra(grid);
    print_grid(grid);
    return 0;
}