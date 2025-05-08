#ifndef CONSTRUCTIVEALGORITHM_HPP
#define CONSTRUCTIVEALGORITHM_HPP

#include <bits/stdc++.h>
#include "Graph.hpp"
using namespace std;
#define ll long long int

double random_choice_prob(){
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0,1.0);

    return dis(gen); 
}




double compute_cut(const set<int>& x, const set<int>& y, const graph& g) {
    double cut_value = 0.0;
    for (const auto& e : g.edges) {
        if ((x.count(e.u) && y.count(e.v)) || (x.count(e.v) && y.count(e.u))) {
            cut_value += e.w;
        }
    }
    return cut_value;
}

double compute_weight(int z, const set<int>& s, const graph& g){
    double weight  = 0.0;
    for(const auto& e : g.edges){
        if((s.count(e.u) and e.v==z) or (s.count(e.v) and e.u == z)) 
          weight+=e.w;
    }
    return weight;
}




class ConstructiveAlgorithm {
    public:
        graph g;
        ConstructiveAlgorithm(const graph& g) : g(g) {}


    double random_heuristic(int num_ieterations) {
        ll total_cut_weight = 0;

        for(int i=0; i<num_ieterations; i++){
            set<int> x,y;
            for(int v=0; v<g.n; v++){
                auto random_choice_w_prob = random_choice_prob();
                if(random_choice_w_prob>=0.5)  x.insert(v);
                else  y.insert(v);
            }
            total_cut_weight+=compute_cut(x,y,g);
        }

        double avg_cut_weight = (1.0*total_cut_weight)/num_ieterations;
        return avg_cut_weight;
    }


    double greedy_heuristic(){
        set<int> x,y;
        sort(g.edges.begin(),g.edges.end());
        
        int u = g.edges[0].u;
        int v = g.edges[0].v;
        
        x.insert(u);
        y.insert(v);

        for(int V=0; V<g.n; V++){
            if(V==v or V==u) continue;
            auto w_x = compute_weight(V,y,g);
            auto w_y = compute_weight(V,x,g);

            if(w_x>w_y)
              x.insert(V);
            else{
                y.insert(V);
            }
        }

        return compute_cut(x,y,g);
    }

};









#endif