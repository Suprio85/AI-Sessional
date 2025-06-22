#ifndef CHAINREACTION_HPP
#define CHAINREACTION_HPP
#include <bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;
typedef vector<pii> vpii;
typedef vector<string> vs;
typedef vector<vs> vvs;
typedef long long ll;

#define COLS 6
#define ROWS 9
#define MAX_DEPTH 10000
#define POS_INF 1e9-1000
#define NEG_INF -1e9+1000

struct cell
{
    int x;
    int y;


    cell() : x(-1), y(-1) {};
    cell(int x, int y) : x(x), y(y) {};
    bool operator==(const cell &other) const
    {
        return x == other.x && y == other.y;
    }
    bool operator<(const cell &other) const
    {
        return x < other.x || (x == other.x && y < other.y);
    }
    cell operator+(const cell &other) const
    {
        return cell(x + other.x, y + other.y);
    }
};


int to_int(const char &c)
{
    return c - '0';
}

char to_char(const int &num)
{
    return num + '0';
}

int color_index(const char &color){
        assert(color == 'R' || color == 'B');
        if (color == 'R') return 0;

        return 1;
}

    
bool is_valid(const cell &c) {
            return c.x >= 0 && c.x < ROWS && c.y >= 0 && c.y < COLS;
    }
 vector<cell> adj_cells = {cell(-1, 0), cell(0, -1), cell(0, 1), cell(1, 0)};
 int neighbor_count(const cell &c)
    {
        int cnt = 0;
        for (const auto &adj : adj_cells)
        {
            cell adj_cell = adj + c;
            if (is_valid(adj_cell))
            cnt++;
        }
        return cnt;
    }




struct board
{
    vvs cells;
    board()
    {
        cells.resize(ROWS, vs(COLS, "0"));
    }
    board(const vvs &initial_cells) : cells(initial_cells)
    {
        if (cells.size() != ROWS || cells[0].size() != COLS)
        {
            throw invalid_argument("Invalid board size");
        }
    }

    void reset()
    {
        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                cells[i][j] = "0";
            }
        }
    }

    bool is_empty(const cell &c) const
    {
        return cells[c.x][c.y] == "0";
    }


    void debug() const {
        for(int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++)
            { 
                cout << cells[i][j] << " ";
            }
            cout << endl;
        }
    }

    bool is_valid_move(const cell &c,const char& player) const
    {
        return is_valid(c) && (is_empty(c) || cells[c.x][c.y][1] == player);
    }

    void print()
    {
        cout << "\n    ";
        for (int col = 0; col < COLS; ++col)
            cout << col << "   ";
        cout << "\n";

        for (int row = 0; row < ROWS; ++row)
        {
            cout << row << "   ";
            for (int col = 0; col < COLS; ++col)
            {
                string cell = cells[row][col];
                if (cell == "0")
                {
                    cout << "--  ";
                }
                else
                {
                    char count = cell[0];
                    char color = cell[1];

                    if (color == 'R')
                        cout << "\033[31m" << count << "R" << "\033[0m  "; // Red
                    else if (color == 'B')
                        cout << "\033[34m" << count << "B" << "\033[0m  "; // Blue
                    else
                        cout << cell << "  ";
                }
            }
            cout << "\n";
        }
        cout << endl;
    }
};



class state
{
    
    // 0 -> R
    // 1 -> B
    public:

    void update()
    {
        while (true)
        {
            if (unstable_cells.empty() or game_ended())
                break;
                
                cell c = unstable_cells.front();
                unstable_cells.pop();

            int reaction_no = neighbor_count(c);
            int atom_no = to_int(game_board.cells[c.x][c.y][0]);
            char current_color = game_board.cells[c.x][c.y][1];

            assert(reaction_no <= atom_no);

            if (reaction_no == atom_no)
            {
                game_board.cells[c.x][c.y] = "0";
                count[color_index(current_color)]--;
            }else{
                auto new_atom_no = atom_no-reaction_no;
                if(new_atom_no>=reaction_no) unstable_cells.push(c);
                game_board.cells[c.x][c.y][0]=to_char(new_atom_no);
            }
            
            for (auto &adj : adj_cells)
            {
                cell new_cell = adj + c;
                if (is_valid(new_cell))
                add_atom(new_cell, current_color, true);
            }
        }
        while (!unstable_cells.empty())
        unstable_cells.pop();
    }
 
    void init(){
        count[0] = count[1] = 0;
        atom [0] = atom[1] = 0;
    }

    char get_player(const cell &c) const
    {
        if (game_board.is_empty(c))
            return '0';
        return game_board.cells[c.x][c.y][1];
    }
    
    
    board game_board;
    queue<cell> unstable_cells;
    int count[2];
    int atom[2];
    bool is_started=false;
    bool game_ended()
    {
        return is_started and (count[0] == 0 or count[1] == 0);
    }
    
    void add_atom(cell c, char color, bool fromupdate = false)
    {
        int color_idx = color_index(color);
        if(!is_started and count[color_idx]>0 and count[color_idx]>0) is_started = true;
        if (!fromupdate) atom[color_idx]++;

        if (game_board.cells[c.x][c.y] == "0"){
            count[color_idx]++;
            game_board.cells[c.x][c.y]=string("1")+color;
        }else{
            char current_color_num = game_board.cells[c.x][c.y][0];
            char current_color = game_board.cells[c.x][c.y][1];
            if (current_color != color)
            {
                count[color_idx]++;
                count[!color_idx]--;
                atom[!color_idx] -= to_int(current_color_num);
                atom[color_idx] += to_int(current_color_num);
            }
            // atom[color_idx]++;
            game_board.cells[c.x][c.y][1] = color;
            game_board.cells[c.x][c.y][0]++;

            if (neighbor_count(c) <= to_int(current_color_num) + 1)
            {
                unstable_cells.push(c);
                update();
            }
        }
    }

    bool is_empty(const cell &c) const
    {
        return game_board.is_empty(c);
    }


    state()
    {
        game_board.reset();
        init();
    }
};

#endif