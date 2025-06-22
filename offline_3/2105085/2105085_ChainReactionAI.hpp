#ifndef CHAINREACTIONAI_HPP
#define CHAINREACTIONAI_HPP
#include <bits/stdc++.h>
#include "2105085_ChainReaction.hpp"
#include "2105085_heruistics.hpp"
using namespace std;
class ChainReactionAI
{
    public :
    int heuristic;
    int max_depth = MAX_DEPTH;

    ChainReactionAI(){
    }
    ChainReactionAI(int h){
        heuristic = h;
    }

 int evaluate(const state &S, const char &player) {
   switch(heuristic) {
       case 1:
           return heuristic1(S, player);
       case 2:
           return heuristic2(S, player);
       case 3:
           return heuristic3(S, player);
       case 4:
           return heuristic4(S, player);
       case 5:
           return heuristic5(S, player);
       case 6:
            return heuristic6(S, player);
       case 7:
           return composite_eval(S, player);
        default:
           return random_heuristic(S, player); 
   }
}

    int min_max(state &S, int depth, int alpha, int beta, bool maximizingPlayer, const char &player)
    {
        if (depth == 0 || S.game_ended())
            return evaluate(S,player);
        
        // cout<< "Depth: " << depth << ", Player: " << player << endl;


        if (maximizingPlayer)
        {
            int maxEval = NEG_INF;
            for (int i = 0; i < ROWS; i++)
            {
                for (int j = 0; j < COLS; j++)
                {
                    cell c(i, j);
                    if (S.is_empty(c) or S.get_player(c)==player){
                        state child = S;
                        child.add_atom(c, player);
                        int eval = min_max(child, depth - 1, alpha, beta, false, player);
                        maxEval = max(maxEval, eval);
                        // cout << "Evaluating cell (" << c.x << "," << c.y << ") with eval: " << eval << endl;
                        // cout<< "MaxEval: " << maxEval << endl;
                        alpha = max(alpha, eval);
                        // cout << "Alpha: " << alpha << ", Beta: " << beta << endl;
                        if (beta <= alpha)
                            break;
                    }
                }
            }
            return maxEval;
        }
        else
        {
            int minEval = POS_INF;
            char opponent = (player == 'R') ? 'B' : 'R';
            for (int i = 0; i < ROWS; i++)
            {
                for (int j = 0; j < COLS; j++)
                {
                    cell c(i, j);
                    if (S.is_empty(c) or S.get_player(c) == opponent)
                    {   state child = S;
                        child.add_atom(c, opponent);
                        // cout<<"Adding atom at (" << c.x << "," << c.y << ") for player: " <<opponent << endl;
                        // child.game_board.debug();
                        int eval = min_max(child, depth - 1, alpha, beta, true, player);
                        minEval = min(minEval, eval);
                        // cout << "Evaluating cell (" << c.x << "," << c.y << ") with eval: " << eval << endl;
                        // cout<< "MinEval: " << minEval << endl;
                        beta = min(beta, eval);
                        if (beta <= alpha)
                            break;
                    }
                }
            }
            return minEval;
        }
    }
     cell best_move(state& current_state, int depth, char player) {
        int  bestValue = NEG_INF;
        cell bestCell(-1, -1);
        cell alter;
        bool found = false;

        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                cell c(i, j);
                if (!current_state.is_empty(c) &&
                    current_state.get_player(c) != player)
                    continue;

                state child = current_state;
                child.add_atom(c, player, false);
                //  cout<<"Adding atom at (" << c.x << "," << c.y << ") for player: " <<player << endl;
                // child.game_board.debug();
                int moveValue = min_max(child,depth - 1, NEG_INF, POS_INF, false, player);
                // cout << "Evaluating cell (" << c.x << "," << c.y << ") with move value: " << moveValue << endl;
                // cout << "Best Value so far: " << bestValue << endl;
                // cout << "Best Cell so far: (" << bestCell.x << "," << bestCell.y << ")\n";

                if (moveValue > bestValue) {
                    bestValue = moveValue;
                    bestCell  = c;
                    found = true;
                }else{
                    alter = c;
                }
            }
        }
        return found ? bestCell : alter; 
    }
    
};
#endif