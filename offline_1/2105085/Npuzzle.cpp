#include <bits/stdc++.h>
#include "helper_function.hpp"
#include "heuristics.hpp"
using namespace std;
ostream &operator<<(ostream &, const vector<vector<int>> &);
istream &operator>>(istream &, vector<vector<int>> &);

struct state
{
    vector<vector<int>> grid;
    state *parent_state;
    int move;
    double priority;
    double herustic_cost;

    state(vector<vector<int>> grid, state *parent, int move, double priority, double heuristic_cost)
    {
        this->grid = grid;
        this->parent_state = parent;
        this->move = move;
        this->priority = priority;
        this->herustic_cost = heuristic_cost;
    }
    state() {}
    state(vector<vector<int>> grid)
    {
        this->grid = grid;
        this->parent_state = nullptr;
        this->move = 0;
        this->priority = 0;
    }
};

struct CompareStatePtr
{
    bool operator()(const state *a, const state *b)
    {
        if (a->priority == b->priority)
        {
            return a->herustic_cost > b->herustic_cost;
        }
        return a->priority > b->priority;
    }
};

class puzzleSolver
{
    int K;
    function<double(vector<vector<int>>)> heuristic_function;
    state initial_state;
    vector<int> dr = {-1, 0, 1, 0};
    vector<int> dc = {0, 1, 0, -1};
    int explored_nodes = 0;
    int expanded_nodes = 0;

public:
    puzzleSolver(int K, function<double(vector<vector<int>>)> heuristic_function, state initial_state)
    {
        this->K = K;
        this->heuristic_function = heuristic_function;
        this->initial_state = initial_state;
    }

    bool isValid(int x, int y)
    {
        return (x >= 0 && x < K && y >= 0 && y < K);
    }

    void setHeuristicFunction(function<double(vector<vector<int>>)> heuristic_function)
    {
        this->heuristic_function = heuristic_function;
    }
    void setInitialState(state initial_state)
    {
        this->initial_state = initial_state;
    }

    bool isSolvable(vector<vector<int>>& grid)
    {
        // odd case
        int inversions = count_inversions(grid);
        if (K & 1)
        {
            return !(inversions & 1);
        }

        // even case
        else
        {
            int blank_row = find_row_of_blank_from_bottom(grid);
            if (!(blank_row & 1) and (inversions & 1))
                return true;
            else if ((blank_row & 1) and !(inversions & 1))
                return true;
        }
        return false;
    }
    bool isGoalState(vector<vector<int>> grid)
    {
        return grid == getTargetGrid(K);
    }

    void printPath(state *current_state)
    {
        vector<state *> path;
        while (current_state != nullptr)
        {
            path.push_back(current_state);
            current_state = current_state->parent_state;
        }
        reverse(path.begin(), path.end());
        for (auto state : path)
        {
            cout << state->grid << endl;
        }
        cout << "Total explored nodes: " << this->explored_nodes << endl;
        cout << "Total expanded nodes: " << this->expanded_nodes << endl;
    }

    void solvePuzzle()
    {

        if (!isSolvable(initial_state.grid))
        {
            cout << "Unsolvable puzzle" << endl;
            return;
        }
        priority_queue<state *, vector<state *>, CompareStatePtr> pq;
        set<vector<vector<int>>> closed_set;
        this->expanded_nodes = 0;
        
        pq.push(&initial_state);
        this->explored_nodes = 1;

        while (!pq.empty())
        {
            state *current_state = pq.top();
            pq.pop();
            
            if (closed_set.count(current_state->grid) > 0)
            continue;
            
            
            if (isGoalState(current_state->grid))
            {
                cout << "Minimum Number of moves: " << current_state->move << endl;
                printPath(current_state);
                break;
            }
            this->expanded_nodes++;

            closed_set.insert(current_state->grid);

            for (int r = 0; r < K; r++)
            {
                for (int c = 0; c < K; c++)
                {

                    if (current_state->grid[r][c] == 0)
                    { // blank

                        for (int i = 0; i < 4; i++)
                        {
                            int next_row = r + dr[i];
                            int next_col = c + dc[i];

                            if (isValid(next_row, next_col))
                            {

                                auto next_grid = current_state->grid;

                                swap(next_grid[r][c], next_grid[next_row][next_col]);

                                int next_move = current_state->move + 1;
                                double next_heuristic_cost = heuristic_function(next_grid);
                                double next_priority = next_move + next_heuristic_cost;
                                // cout << "Next move: " << next_move << endl;
                                // cout << "Next heuristic cost: " << next_heuristic_cost << endl;
                                // cout << "Next priority: " << next_priority << endl;

                                if (closed_set.count(next_grid))
                                    continue;

                                state *next_state = new state(next_grid, current_state, next_move, next_priority, next_heuristic_cost);
                                pq.push(next_state);
                                this->explored_nodes++;
                            }
                        }
                    }
                }
            }
        }
    }
};

int K;
int main()
{

    cin >> K;
    vector<vector<int>> grid(K, vector<int>(K, 0));
    cin >> grid;
    state initial_state(grid);
     puzzleSolver solver(K,hammingDistance,initial_state);
    solver.solvePuzzle();
}

ostream &operator<<(ostream &os, const vector<vector<int>> &grid)
{
    for (const auto &row : grid)
    {
        for (const auto &elem : row)
        {
            os << elem << " ";
        }
        os << endl;
    }
    return os;
}
istream &operator>>(istream &is, vector<vector<int>> &grid)
{
    for (auto &row : grid)
    {
        for (auto &elem : row)
        {
            is >> elem;
        }
    }
    return is;
}