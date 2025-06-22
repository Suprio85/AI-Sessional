#include "ChainReaction.hpp"
#include "ChainReactionAI.hpp"
#include "heruistics.hpp"
#include <bits/stdc++.h>
#include <chrono>
#include <fstream>
#include <thread>

using namespace std;
using namespace std::chrono;

int play_ai_vs_ai(char player1, char player2, int heuristic1, int heuristic2, int depth1, int depth2) {
    ChainReactionAI AI1(heuristic1);
    ChainReactionAI AI2(heuristic2);
    state S;
    S.game_board.reset();
    char current_player = player1;
    while (!S.game_ended()) {
        cell move = (current_player == player1) ? AI1.best_move(S, depth1, player1)
                                               : AI2.best_move(S, depth2, player2);
        S.add_atom(move, current_player);
        current_player = (current_player == player1) ? player2 : player1;
    }
    if (S.count[0] == 0 && S.count[1] > 0)
        return -1; // Player2 (Blue) wins
    else if (S.count[1] == 0 && S.count[0] > 0)
        return 1; // Player1 (Red) wins
    else
        return 0; // Tie
}

int randomagent_vs_AI(char player1, char player2, int heuristics, int depth) {
    ChainReactionAI AI(heuristics);
    state S;
    S.game_board.reset();
    char ai_color = player1;
    char random_color = player2;

    while (!S.game_ended()) {
        cell ai_move = AI.best_move(S, depth, ai_color);
        S.add_atom(ai_move, ai_color);

        if (S.game_ended()) break;
        cell random_move(rand() % ROWS, rand() % COLS);
        while (!S.game_board.is_valid_move(random_move, random_color)) {
            random_move = cell(rand() % ROWS, rand() % COLS);
        }
        S.add_atom(random_move, random_color);
    }
    if (S.count[0] == 0 && S.count[1] > 0)
        return -1; // Random (Blue) wins
    else if (S.count[1] == 0 && S.count[0] > 0)
        return 1; // AI (Red) wins
    else
        return 0; // Tie
}

void write_csv_header(ofstream &fout) {
    fout << "Experiment_Type,Heuristic_AI1,Depth_AI1,Heuristic_AI2,Depth_AI2,AI1_Win_Rate,Avg_Game_Duration_Seconds,Total_Game_Time_Seconds,Num_Games\n";
}

void write_csv_row(ofstream &fout, const string &exp_type, int h1, int d1, int h2, int d2, double win_rate, double avg_duration, double total_duration, int num_games) {
    fout << exp_type << "," << h1 << "," << d1 << "," << h2 << "," << d2 << ","
         << fixed << setprecision(1) << win_rate << "," << avg_duration << "," << total_duration << "," << num_games << "\n";
}

double play_single_game(char player1, char player2, int h1, int h2, int depth1, int depth2, bool is_random = false) {
    auto start = high_resolution_clock::now();
    int is_win;
    if (is_random) {
        is_win = randomagent_vs_AI(player1, player2, h1, depth1);
    } else {
        is_win = play_ai_vs_ai(player1, player2, h1, h2, depth1, depth2);
    }
    auto end = high_resolution_clock::now();
    double duration = duration_cast<microseconds>(end - start).count() / 1e6;
    return is_win * duration;
}

void run_experiment(const string &exp_type, int h1, int d1, int h2, int d2, ofstream &fout, bool is_random = false) {
    const int num_games = 5;
    int ai1_wins = 0;
    double total_duration = 0.0;
    char player1 = 'R';
    char player2 = 'B';
    seed_random();

    if (exp_type == "AI_vs_AI") {
        const int num_opponents = 6; 
        int total_games = num_opponents;
        vector<int> opponents;
        for (int h = 1; h <= 7; ++h) {
            if (h != h1) opponents.push_back(h);
        }

        for (int h2 : opponents) {
                double duration = play_single_game(player1, player2, h1, h2, d1, d1, false);
                if (duration >= 0) ++ai1_wins;
                total_duration += abs(duration);
        }
        double win_rate = (ai1_wins * 100.0) / total_games;
        double avg_duration = total_duration / total_games;
        write_csv_row(fout, exp_type, h1, d1, h2, d2, win_rate, avg_duration, total_duration, total_games);
    } else {
        for (int i = 0; i < num_games; ++i) {
            double duration = play_single_game(player1, player2, h1, h2, d1, d2, is_random);
            if (duration >= 0) ++ai1_wins;
            total_duration += abs(duration);
        }
        double win_rate = (ai1_wins * 100.0) / num_games;
        double avg_duration = total_duration / num_games;
        write_csv_row(fout, exp_type, h1, d1, is_random ? -1 : h2, is_random ? -1 : d2, win_rate, avg_duration, total_duration, num_games);
    }
}

int main() {
    ofstream fout("experiment_results2.csv");
    if (!fout) {
        cerr << "Error opening CSV file for writing\n";
        return 1;
    }

    write_csv_header(fout);
    // for (int h = 1; h <= 7; ++h) {
    //     for (int d = 1; d <= 3; ++d) {
    //         run_experiment("AI_vs_Human", h, d, 2, 1, fout);
    //     }
    // }

    for (int h1 = 1; h1 <= 7; ++h1) {
        for (int d = 1; d <= 3; ++d) {
            run_experiment("AI_vs_AI", h1, d, -1, -1, fout);
        }
    }

    // for (int h = 1; h <= 7; ++h) {
    //     run_experiment("Random_vs_AI", h, 3, -1, -1, fout, true);
    // }

    fout.close();
    cout << "Experiment results written to experiment_results.csv\n";
    return 0;
}