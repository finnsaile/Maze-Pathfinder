#include <iostream>
#include <fstream>
#include <stack>
#include <vector>
#include <cstdlib>
//#include <stdexcept>

using namespace std;

typedef pair<int64_t, int64_t> iPair;

struct Node
{
    Node(const int64_t _value, const iPair _cord): cord {_cord},  value {_value}{}
    int64_t value; 
    iPair cord;
    bool visited = false;
};

/**
 * @brief generates maze using depth first algorithm
 * Starts at certain node, marking it as visited and chosing a viable neighbor to move on to, and destroy wall on way to member.
 * Reapeats this process until no viable neighbor is found. Backtracks until viable neighbor is found again.
 * Repeats process until whole grid has been visited
 * 
 * @param grid array that contains the maze
 * @param size_y y size of maze
 * @param size_x x size of maze
 */
void generate_maze(vector<vector<Node>>& grid, const int64_t& size_y, const int64_t& size_x)
{
    //vector containing relative coordinates of all possible neighbors
    const vector<vector<int64_t>> neighbor_vec = {{0, 2}, {0, -2}, {2, 0}, {-2, 0}};
    //variable to safe relative coordinates of chosen neighbor
    vector<int64_t> neighbor;
    //stack being used for backtracking once dead end is reached
    stack<Node> node_stack;

    //push start point on stack
    node_stack.push(grid[1][1]);

    //run while stack is not empty(ensures that avery point in grid is visited)
    while(!node_stack.empty())
    {   
        //get coordinates of highest stack member
        auto [y, x] = node_stack.top().cord;
        //mark current node as visited
        grid[y][x].visited = true;
        //create copy of possible neighbors
        auto temp_neighbor_vec = neighbor_vec;
        
        //chose random neighbor
        while(!temp_neighbor_vec.empty())
        {
            //get index of neighbor 
            int64_t rand_num = arc4random() % temp_neighbor_vec.size();
            //get coordinates of neighbor
            neighbor = temp_neighbor_vec[rand_num];
            //check if chosen neighbor is out of bounds or has been visited already
            if(y + neighbor[0] < 1 || x + neighbor[1] < 1 || y + neighbor[0] > size_y - 2 || x + neighbor[1] > size_x - 2 || grid[y + neighbor[0]][x + neighbor[1]].visited == true)
                //delete neighbor from temp vector if it cant be visited
                temp_neighbor_vec.erase(next(temp_neighbor_vec.begin(), rand_num));
            //viable neighbor has been found 
            else
            {
                //set value of wall between current value and neighbor to 1 
                //(this value will however not be used as a future neighbor, 
                //as its coordinates don't fullfill the criteria of a accesable tile)
                grid[y + (neighbor[0] / 2)][x + (neighbor[1] / 2)].value = 1;
                //push neighbor onto stack meaning it will be visited next
                node_stack.push(grid[y + neighbor[0]][x + neighbor[1]]);
                break;
            }       
        }

        //if no viable neighbor has been found pop element(which causes backtracking)
        if(temp_neighbor_vec.empty())
            node_stack.pop();
    }
}

int main(int argc, char* argv[])
{
    int64_t size_y, size_x;

    //check if required size arguments were given
    if(argc < 3)
        throw invalid_argument("Not enough arguments given.");
    //read size arguments
    size_y = stoi(argv[1]);
    size_x = stoi(argv[2]);
    //check if size is odd and not negative
    if(size_x % 2 == 0 || size_y % 2 == 0)
        throw invalid_argument("Size coordinates must be odd values.");
    if(size_x <= 0 || size_y <= 0)
        throw out_of_range("Size coordinates must be greater than zero.");

    //create gird
    vector<vector<Node>> grid;
    
    //create grid of 0 and 1, where each 1 is outline by 8 0
    for(int64_t i = 0; i < size_y; i++)
    {
        grid.push_back(vector<Node>());
        for(int64_t j = 0; j < size_x; j++)
        {
            if(i % 2 == 0 || j % 2 == 0)
                grid.back().push_back(Node(0, iPair(i, j)));
            else 
                grid.back().push_back(Node(1, iPair(i, j)));
        }
    }

    //generate maze with given size
    generate_maze(grid, size_y, size_x);

    ofstream file;
    //print maze to file
    if(argc >= 4)
        file.open(argv[3]);
    else
        file.open("maze.txt");

    for(auto& row: grid)
    {
        for(auto& col: row)
            file.put(col.value + '0');
        file.put('\n');
    }
    file.close();
}


