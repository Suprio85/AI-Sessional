#include "ChainReaction.hpp"
#include "heruistics.hpp"
#include "ChainReactionAI.hpp"
#include<bits/stdc++.h>
#include<stdlib.h>


using namespace std;

const string filename = "gamestate.txt";

board parse_board(ifstream &fin) {
    vvs cells(ROWS, vs(COLS));
    for (int i = 0; i < ROWS; ++i) {
        for(int j = 0; j < COLS; ++j) {
            string cell_str;
            fin >> cell_str;
            // cout << "Read cell (" << i << "," << j << "): " << cell_str << endl;
            cells[i][j] = cell_str;
        }
    }
    return board(cells);
}


void write_board(ofstream &fout, const string &who, const board &b) {
    fout << who << "\n";
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            fout << b.cells[i][j] << " ";
        }
        fout << "\n";
    }
    fout.flush();
}

void wait_for_turn(const string &expected_header) {
    while (true) {
        ifstream fin(filename);
        string header;
        getline(fin, header);
        fin.close();
        if (header == expected_header) break;
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
}
void play_ai_vs_ai(int heuristic1, int heuristic2, int depth1,int depth2) {
    ChainReactionAI AI1(heuristic1);
    ChainReactionAI AI2(heuristic2);
    state S;
    S.game_board.reset();
    char current_player = 'R';

    S.game_board.print();
    while (!S.game_ended()) {
        cout<<"AI " << current_player << " is thinking...\n";
        cell move = (current_player == 'R') ? AI1.best_move(S, depth1, 'R')
                                           : AI2.best_move(S, depth2, 'B');
        S.add_atom(move, current_player);
        cout << "AI " << current_player << " placed at (" << move.x << "," << move.y << ")\n";
        S.game_board.print();
        current_player = (current_player == 'R') ? 'B' : 'R';
        this_thread::sleep_for(chrono::milliseconds(500));
       
    }

    S.game_board.print();
    if (S.count[0] == 0 && S.count[1] > 0)
        cout << "Blue wins!\n";
    else if (S.count[1] == 0 && S.count[0] > 0)
        cout << "Red wins!\n";
    else
        cout << "It's a tie!\n";
}

void human_vs_ai(int heuristic, int depth) {
    ChainReactionAI AI(heuristic);
    char ai_color = 'B';
    char human_color = 'R';
     
    state S;
    S.game_board.reset();
    S.game_board.print();

    ofstream fout(filename);
    if (!fout) {
        cerr << "Error opening file for writing: " << filename << endl;
        return ;
    }
    write_board(fout, "Initial move:", S.game_board);
    

    while (!S.game_ended()) {
        ifstream fin(filename);
        string header;
        getline(fin, header);
        if (header == "Human Move:") {
            cout << "Waiting for AI move...\n";
            wait_for_turn("AI Move:");
        }
        board current_board = parse_board(fin);
        S.game_board = current_board;
        // S.game_board.debug();

        cout << "Enter your move (row col): ";
        int r, c;
        cin >> r >> c;
        while(!S.game_board.is_valid_move(cell(r, c), human_color)) {
            cout << "Invalid move. Try again.\n";
            cout << "Enter your move (row col): ";
            cin >> r >> c;
        }
        S.add_atom(cell(r, c), human_color);
        S.game_board.print();

        ofstream fout1(filename);
        write_board(fout1, "Human Move:", S.game_board);
        fout1.close();

        // Read From File
        ifstream fin1(filename);
        header;
        getline(fin1, header);
        if(header == "AI Move:"){
            cout <<"Waiting for Human move...\n";
            wait_for_turn("Human Move:");
        }
        current_board = parse_board(fin1);
        S.game_board = current_board;
        cout<<"AI is thinking...\n";
        auto ai_move = AI.best_move(S, depth, ai_color);
        S.add_atom(ai_move, ai_color);
        cout << "AI placed at (" << ai_move.x << "," << ai_move.y << ")\n";
        S.game_board.print();
        
        ofstream fout2(filename);
        write_board(fout2, "AI Move:", S.game_board);
        fout2.close();
    }

    S.game_board.print();
    cout << "Game ended.\n";
    if (S.count[0] == 0 && S.count[1] > 0)
        cout << "Blue wins!\n";
    else if (S.count[1] == 0 && S.count[0] > 0)
        cout << "Red wins!\n";
    else
        cout << "It's a tie!\n";
}

void randomagent_vs_AI(int heuristics, int depth){
    ChainReactionAI AI(heuristics);
    state S;
    S.game_board.reset();
    char ai_color = 'B';
    char random_color = 'R';

    S.game_board.print();
    while (!S.game_ended()) {
        cout << "AI "<<ai_color<<  " is thinking...\n";
        cell ai_move = AI.best_move(S, depth, ai_color);
        S.add_atom(ai_move, ai_color);
        cout << "AI placed at (" << ai_move.x << "," << ai_move.y << ")\n";
        S.game_board.print();

        if (S.game_ended()) break;

        cout << "Random agent "<<random_color<< " is thinking...\n";
        cell random_move(rand() % ROWS, rand() % COLS);
        while (!S.game_board.is_valid_move(random_move, random_color)) {
            random_move = cell(rand() % ROWS, rand() % COLS);
        }
        S.add_atom(random_move, random_color);
        cout << "Random agent placed at (" << random_move.x << "," << random_move.y << ")\n";
        S.game_board.print();
         this_thread::sleep_for(chrono::milliseconds(800));

    }

    S.game_board.print();
    if (S.count[0] == 0 && S.count[1] > 0)
        cout << "Blue wins!\n";
    else if (S.count[1] == 0 && S.count[0] > 0)
        cout << "Red wins!\n";
    else
        cout << "It's a tie!\n";

}




int main() {
    seed_random();
    char ai_color = 'B';
    char human_color = 'R';
    ChainReactionAI AI(7);

    cout << "Choose mode:\n1. Human vs AI\n2. AI vs AI\n3. Random vs AI\n> ";
    int mode;
    cin >> mode;

    if (mode == 1) {
        cout<<"Enter heuristic for AI (1-7): ";
        int heuristic;
        cin >> heuristic;
        int depth;
        cout << "Enter search depth: ";
        cin >> depth;
        cout << "You are playing as " << human_color << ". AI is " << ai_color << ".\n";
        human_vs_ai(heuristic,depth);
    } else if (mode == 2) {
        int heuristic1, heuristic2, depth1,depth2;
        cout << "Enter heuristic for AI1 (1-7): ";
        cin >> heuristic1;
        cout << "Enter search depth for AI1: ";
        cin >> depth1;
        cout << "Enter heuristic for AI2 (1-7): ";
        cin >> heuristic2;
        cout <<"Enter search depth for AI2: ";
        cin>>depth2;
        play_ai_vs_ai(heuristic1, heuristic2, depth1,depth2);
    } else if(mode==3){
        int heuristics,depth;
        cout<<"Enter heuristics for AI1(1-7): ";
        cin>> heuristics;
        cout<< "Enter depth: ";
        cin>> depth;
        randomagent_vs_AI(heuristics,depth);
    }
   

    return 0;
}
