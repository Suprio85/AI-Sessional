#include "ChainReaction.hpp"
#include "ChainReactionAI.hpp"

#include <iostream>

using namespace std;

// How deep the AI should search. You can tune this (e.g., 3 or 4).
static const int AI_DEPTH = 3;

// -----------------------------------------------------------------------------
// Read a move from the human player (only Red). No validation beyond bounds.
// -----------------------------------------------------------------------------
cell read_human_move() {
    int r, c;
    while (true) {
        cout << "Red's turn. Enter row and column (0-" << ROWS-1
             << " 0-" << COLS-1 << "): ";
        cin >> r >> c;
        return cell(r, c);
    }
}

// -----------------------------------------------------------------------------
// Main: Human (Red) vs AI (Blue)
// -----------------------------------------------------------------------------
int main() {
    state S;                           // Game state
    ChainReactionAI AI_engine(6);
    ChainReactionAI AI_engine2(3); // Use heuristic 1 for AI
    char turn = 'R';                   // Red (human) starts
    
    cout << "\n=== Chain Reaction: Human (Red) vs AI (Blue) ===\n";
    S.game_board.print();

    while (true) {
        // 1) Check for game end before asking for a move
        if (S.game_ended()) {
            S.game_board.print();
            if (S.count[0] == 0 && S.count[1] == 0) {
                cout << "  Game over: nobody has any orbs left. It's a tie?\n";
            }
            else if (S.count[0] == 0) {
                cout << "\n*** Blue (AI) wins! ***\n";
            }
            else { // S.count[1] == 0
                cout << "\n*** Red (You) win! ***\n";
            }
            break;
        }

        if (turn == 'R') {
            cout << "--- Red (AI 1) is thinking …\n";
            cell ai_mv = AI_engine2.best_move(S, AI_DEPTH, 'R');
          
                S.add_atom(ai_mv, 'R', false);
                cout << "\nRed placed at (" << ai_mv.x << "," << ai_mv.y << ")\n";
                S.game_board.print();
                cout << "Red owns "  << S.count[0] << " cells and " << S.atom[0] << " orbs.\n";
                cout << "Blue owns " << S.count[1] << " cells and " << S.atom[1] << " orbs.\n\n";
            

            // Update the AI’s internal state
            turn = 'B';
        }
        else {
            // ----- AI (Blue) move -----
            cout << "--- AI (Blue) is thinking (depth=" << AI_DEPTH << ") …\n";
            cell ai_mv = AI_engine.best_move(S, AI_DEPTH, 'B');
          
                S.add_atom(ai_mv, 'B', false);
                cout << "\nAI placed at (" << ai_mv.x << "," << ai_mv.y << ")\n";
                S.game_board.print();
                cout << "Red owns "  << S.count[0] << " cells and " << S.atom[0] << " orbs.\n";
                cout << "Blue owns " << S.count[1] << " cells and " << S.atom[1] << " orbs.\n\n";
            // Sync AI’s internal state again
            turn = 'R';
        }
    }

    return 0;
}
