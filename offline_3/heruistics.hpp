#ifndef HERUISTICS_HPP
#define HERUISTICS_HPP

#include <bits/stdc++.h>
#include "ChainReaction.hpp"
using namespace std;

int heuristic5(const state &S, const char &player)
{
    int pidx = color_index(player);
    int oppidx = 1 - pidx;
    // S.game_board.debug();

    if (S.count[pidx] == 0 && S.count[oppidx] > 0)
        return NEG_INF;
    if (S.count[oppidx] == 0 && S.count[pidx] > 0)
        return POS_INF;

    int atom_diff = S.atom[pidx] - S.atom[oppidx];
    int count_diff = S.count[pidx] - S.count[oppidx];

    int my_cp = 0;
    int opp_cp = 0;
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            const string &cellStr = S.game_board.cells[r][c];
            if (cellStr == "0")
                continue;

            int n = to_int(cellStr[0]);
            char col = S.get_player(cell(r, c));
            int crit = neighbor_count(cell(r, c));

            if (n == crit - 1)
            {
                if (col == player)
                {
                    for (auto &adj : adj_cells)
                    {
                        cell d = adj + cell(r, c);
                        if (!is_valid(d))
                            continue;
                        string adjStr = S.game_board.cells[d.x][d.y];
                        if (adjStr != "0" && adjStr[1] != player)
                        {
                            // cout<< "Potential conversion at (" << r << "," << c << ") to " << adjStr << endl;
                            my_cp++;
                        }
                    }
                }
                else if (col != '0')
                {
                    for (auto &adj : adj_cells)
                    {
                        cell d = adj + cell(r, c);
                        if (!is_valid(d))
                            continue;
                        string adjStr = S.game_board.cells[d.x][d.y];
                        if (adjStr != "0" && adjStr[1] == player)
                        {
                            // cout<< "Potential conversion at (" << r << "," << c << ") to " << adjStr << endl;
                            opp_cp++;
                        }
                    }
                }
            }
        }
    }
    // cout<<"potential: "<<my_cp<<" "<<opp_cp<<endl;
    // cout<<"difference: "<<atom_diff<<" "<<my_cp<<" "<<opp_cp<<endl;
    // cout<<"Best move evaluation: "<<atom_diff + 4 * (my_cp - opp_cp)<<endl;
    int total_orbs = S.atom[pidx] + S.atom[oppidx];
    int W;
    if (total_orbs < 20)
        W = 2; // Early game
    else if (total_orbs < 40)
        W = 5; // Mid game
    else
        W = 8; // Late game
    W += abs(atom_diff) / 2;
    return atom_diff + W * (my_cp - opp_cp);
}

int heuristic4(const state &S, const char &player)
{
    int pidx = color_index(player);
    int oppidx = 1 - pidx;

    if (S.count[pidx] == 0 && S.count[oppidx] > 0)
        return NEG_INF;
    if (S.count[oppidx] == 0 && S.count[pidx] > 0)
        return POS_INF;

    int my_cp = 0;
    int opp_cp = 0;
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            const string &cellStr = S.game_board.cells[r][c];
            if (cellStr == "0")
                continue;

            int n = to_int(cellStr[0]);
            char col = S.get_player(cell(r, c));
            int crit = neighbor_count(cell(r, c));

            if (n == crit - 1)
            {
                if (col == player)
                {
                    for (auto &adj : adj_cells)
                    {
                        cell d = adj + cell(r, c);
                        if (!is_valid(d))
                            continue;
                        string adjStr = S.game_board.cells[d.x][d.y];
                        if (adjStr != "0" && adjStr[1] != player)
                        {
                            // cout<< "Potential conversion at (" << r << "," << c << ") to " << adjStr << endl;
                            my_cp++;
                        }
                    }
                }
                else if (col != '0')
                {
                    for (auto &adj : adj_cells)
                    {
                        cell d = adj + cell(r, c);
                        if (!is_valid(d))
                            continue;
                        string adjStr = S.game_board.cells[d.x][d.y];
                        if (adjStr != "0" && adjStr[1] == player)
                        {
                            // cout<< "Potential conversion at (" << r << "," << c << ") to " << adjStr << endl;
                            opp_cp++;
                        }
                    }
                }
            }
        }
    }

    return (my_cp - opp_cp);
}

int heuristic3(const state &S, const char &player)
{
    int pidx = color_index(player);
    int oppidx = 1 - pidx;

    if (S.count[pidx] == 0 && S.count[oppidx] > 0)
        return NEG_INF;
    if (S.count[oppidx] == 0 && S.count[pidx] > 0)
        return POS_INF;

    int my_threats = 0;
    int opp_threats = 0;

    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            const string &cellStr = S.game_board.cells[r][c];
            if (cellStr == "0")
                continue;

            int n = to_int(cellStr[0]);
            char col = cellStr[1];
            int crit = neighbor_count(cell(r, c));

            if (n == crit - 1)
            {
                if (col == player)
                    my_threats++;
                else if (col != '0')
                    opp_threats++;
            }
        }
    }
    return 2 * my_threats - opp_threats;
}

int heuristic2(const state &S, const char &player)
{
    int score = 0;
    const int F = 3;
    const int G = 5;
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            string s = S.game_board.cells[i][j];
            if (s == "0")
                continue;
            int n = to_int(s[0]);
            char col = s[1];
            int crit = neighbor_count(cell(i, j));
            for (auto &d : adj_cells)
            {
                cell nb(i + d.x, j + d.y);
                if (!is_valid(nb))
                    continue;
                string t = S.game_board.cells[nb.x][nb.y];
                if (t == "0")
                    continue;
                char ncol = t[1];
                int nn = to_int(t[0]);
                if (col == player && ncol != player)
                {
                    score += F;
                }
                if (col != player && ncol == player)
                {
                    if (n >= crit - 1)
                    {
                        score -= G;
                    }
                }
            }
        }
    }
    return score;
}

int heuristic1(const state &S, const char &player)
{
    int pidx = color_index(player);
    int oppidx = 1 - pidx;
    if (S.count[pidx] == 0 && S.count[oppidx] > 0)
        return NEG_INF;
    if (S.count[oppidx] == 0 && S.count[pidx] > 0)
        return POS_INF;

    int my_stability = 0;
    int opp_stability = 0;

    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            const string &cellStr = S.game_board.cells[r][c];
            if (cellStr == "0")
                continue;

            int n = to_int(cellStr[0]);
            char col = cellStr[1];
            int crit = neighbor_count(cell(r, c));
            int dist = crit - n;

            if (col == player)
            {
                my_stability += dist;
            }
            else
            {
                opp_stability += dist;
            }
        }
    }
    const int D = 2;
    return D * (my_stability - opp_stability);
}

int heuristic6(const state &S, const char &player)
{
    int pidx = color_index(player);
    int oppidx = 1 - pidx;

    if (S.count[pidx] == 0 && S.count[oppidx] > 0)
        return NEG_INF;
    if (S.count[oppidx] == 0 && S.count[pidx] > 0)
        return POS_INF;

    int score = 0;

    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            string cellStr = S.game_board.cells[r][c];
            if (cellStr == "0")
            {

                for (auto &adj : adj_cells)
                {
                    cell nb = cell(r, c) + adj;
                    if (!is_valid(nb))
                        continue;
                    string adjStr = S.game_board.cells[nb.x][nb.y];
                    if (adjStr != "0" && adjStr[1] != player && to_int(adjStr[0]) >= neighbor_count(nb) - 2)
                    {
                        score -= 100;
                        break;
                    }
                }
            
            }

            int n = to_int(cellStr[0]);
            char col = cellStr[1];
            cell pos(r, c);
            int crit = neighbor_count(pos);

            if (r >= 2 && r <= ROWS - 3 && c >= 2 && c <= COLS - 3)
            {
                if (cellStr == "0" || col == player)
                    score += 20;
            }

            if ((r == 0 || r == ROWS - 1) && (c == 0 || c == COLS - 1))
            {
                if (col == player || cellStr == "0")
                    score += 1250;
            }

            else if (r == 0 || c == 0 || r == ROWS - 1 || c == COLS - 1)
            {
                if (col == player || cellStr == "0")
                    score += 1180;
            }

                if (col == player && n == crit - 1)
                {
                    int explosion_chain = 1;
                    for (auto &adj : adj_cells)
                    {
                        cell nb = pos + adj;
                        if (!is_valid(nb))
                            continue;
                        string s = S.game_board.cells[nb.x][nb.y];
                        if (s == "0")
                            continue;
                        int nb_n = to_int(s[0]);
                        int nb_crit = neighbor_count(nb);
                        if (nb_n == nb_crit - 1 && s[1] != player)
                        {
                            explosion_chain++;
                        }
                    }
                    score += 1000 * explosion_chain;
                    // continue;
                }

               else if (col == player && n == crit - 2)
                {
                    score += 300;
                    // continue;
                }
            

            // Near opponent threats
            if (col == player || cellStr == "0")
            {
                int my_threats = 0;
                int opp_threats = 0;

                for (auto &d : adj_cells)
                {
                    cell nb = pos + d;
                    if (!is_valid(nb))
                        continue;

                    string nbStr = S.game_board.cells[nb.x][nb.y];
                    if (nbStr == "0")
                        continue;

                    char nbCol = nbStr[1];
                    int nbVal = to_int(nbStr[0]);
                    int nbCrit = neighbor_count(nb);

                    if (nbCol != player && nbVal == nbCrit - 1 && n < crit - 1)
                    {
                        score -= 100;
                    }
                    if (nbCol == player && nbVal == nbCrit - 1)
                    {
                        my_threats++;
                    }
                    else if (nbCol != player && nbCol != '0' && nbVal == nbCrit - 1)
                    {
                        opp_threats++;
                    }
                }

                score += 10 * (opp_threats - my_threats);
            }
        }
    }

    return score;
}

void seed_random() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}
int random_heuristic(const state &S, const char &player) {
    int pidx = color_index(player);
    int oppidx = 1 - pidx;

    if (S.count[pidx] == 0 && S.count[oppidx] > 0)
        return NEG_INF;
    if (S.count[oppidx] == 0 && S.count[pidx] > 0)
        return POS_INF;

    return (std::rand() % 201) - 100; 
}

int composite_eval(const state &S, const char &player)
{
    return 3 * heuristic5(S, player) + 2 * heuristic4(S, player) + 1 * heuristic1(S, player);
}

#endif
