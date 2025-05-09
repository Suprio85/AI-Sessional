#ifndef CONSTRUCTIVEALGORITHM_HPP
#define CONSTRUCTIVEALGORITHM_HPP

#include <bits/stdc++.h>
#include "Graph.hpp"
using namespace std;
#define ll long long int

const double INF = DBL_MAX - 100;

double random_choice_prob()
{
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);

    return dis(gen);
}

double compute_cut(const set<int> &x, const set<int> &y, const graph &g)
{
    double cut_value = 0.0;
    for (const auto &e : g.edges)
    {
        if ((x.count(e.u) && y.count(e.v)) || (x.count(e.v) && y.count(e.u)))
        {
            cut_value += e.w;
        }
    }
    return cut_value;
}

double compute_weight(int z, const set<int> &s, const graph &g)
{
    double weight = 0.0;
    for (const auto &[w, v] : g.adj[z])
    {
        if (s.count(v))
            weight += w;
    }

    return weight;
}

class ConstructiveAlgorithm
{
public:
    graph g;
    ConstructiveAlgorithm(const graph &g) : g(g) {}

    double random_heuristic(int num_ieterations)
    {
        ll total_cut_weight = 0;

        for (int i = 0; i < num_ieterations; i++)
        {
            set<int> x, y;
            for (int v = 0; v < g.n; v++)
            {
                auto random_choice_w_prob = random_choice_prob();
                if (random_choice_w_prob >= 0.5)
                    x.insert(v);
                else
                    y.insert(v);
            }
            total_cut_weight += compute_cut(x, y, g);
        }

        double avg_cut_weight = (1.0 * total_cut_weight) / num_ieterations;
        return avg_cut_weight;
    }

    double greedy_heuristic(set<int> &x, set<int> &y)
    {
        int u = g.edges[0].u;
        int v = g.edges[0].v;

        x.insert(u);
        y.insert(v);

        for (int V = 0; V < g.n; V++)
        {
            if (V == v or V == u)
                continue;
            auto w_x = compute_weight(V, y, g);
            auto w_y = compute_weight(V, x, g);

            if (w_x > w_y)
                x.insert(V);
            else
            {
                y.insert(V);
            }
        }

        return compute_cut(x, y, g);
    }

    // semi greedy with rcl values
    double semi_greedy_heuristics(double alpha, set<int> &x, set<int> &y)
    {

        double w_min = g.edges.back().w;
        double w_max = g.edges[0].w;
        double mu = w_min + alpha * (w_max - w_min);

        vector<edge> rcl_edges;

        for (auto edge : g.edges)
        {
            if (edge.w >= mu)
                rcl_edges.push_back(edge);
            else
                break;
        }

        set<int> v_prime;
        for (int i = 0; i < g.n; i++)
        {
            v_prime.insert(i);
        }

        int idx = rand() % rcl_edges.size();
        auto edge = rcl_edges[idx];
        x.insert(edge.u);
        y.insert(edge.v);
        v_prime.erase(edge.u);
        v_prime.erase(edge.v);

        while (!v_prime.empty())
        {

            vector<tuple<int, double, double>> vertex_sigmas;

            double sigma_min_x = INF, sigma_max_x = -INF;
            double sigma_min_y = INF, sigma_max_y = -INF;

            for (auto v : v_prime)
            {
                double sigma_x = compute_weight(v, y, g);
                double sigma_y = compute_weight(v, x, g);
                sigma_min_x = min(sigma_min_x, sigma_x);
                sigma_min_y = min(sigma_min_y, sigma_y);
                sigma_max_x = max(sigma_max_x, sigma_x);
                sigma_max_y = max(sigma_max_y, sigma_y);
                vertex_sigmas.emplace_back(v, sigma_x, sigma_y);
            }

            auto w_min = min(sigma_min_x, sigma_min_y);
            auto w_max = max(sigma_max_x, sigma_max_y);

            mu = w_min + alpha * (w_max - w_min);

            vector<int> RCL;
            for (const auto &[v, sigma_x, sigma_y] : vertex_sigmas)
            {
                auto greedy_func_value = max(sigma_x, sigma_y);
                if (greedy_func_value >= mu)
                {
                    RCL.push_back(v);
                }
            }

            if (RCL.empty())
            {
                for (const auto &[v, _, __] : vertex_sigmas)
                {
                    RCL.push_back(v);
                }
            }

            int z = RCL[rand() % RCL.size()];
            double z_sigma_x = 0, z_sigma_y = 0;
            for (const auto &[v, sigma_x, sigma_y] : vertex_sigmas)
            {

                if (v == z)
                {
                    z_sigma_x = sigma_x;
                    z_sigma_y = sigma_y;
                    break;
                }
            }

            if (z_sigma_x > z_sigma_y)
            {
                x.insert(z);
            }
            else
            {
                y.insert(z);
            }
            v_prime.erase(z);
        }

        return compute_cut(x, y, g);
    }

    pair<double, int> local_search(set<int>& x, set<int>& y) {
       bool improved = true;
       int num_iterations = 0;

       while(improved){
        num_iterations++;
        cout<<num_iterations<<endl;
        improved = false;

        for(int v=0; v<g.n; v++){
            bool Inx = x.count(v);
            
            double sigma_s = compute_weight(v,y,g);
            double sigma_sc = compute_weight(v,x,g);

            auto delta = Inx ? (sigma_sc-sigma_s) : (sigma_s-sigma_sc);
            // cout<<"Inx: "<<Inx<<endl;
            // cout<<"sigma_s "<<sigma_s<<endl;
            // cout<<"sigma_sc "<<sigma_sc<<endl;
            // cout<<"delta "<<delta<<endl;

            if(delta>0){
                improved =true;
                if(Inx){
                    x.erase(v);
                    y.insert(v);

                }else{
                    y.erase(v);
                    x.insert(v);
                }
            }
        }
       }

       return {compute_cut(x,y,g),num_iterations};
       
    }
    double GRASP(int num_iterations, double alpha, int type = 1)
    {

        double best_cut = -INF;
        set<int> best_s;
        set<int> best_sc;

        for (int i = 0; i < num_iterations; i++)
        {
            set<int> s;
            set<int> sc;
            semi_greedy_heuristics(alpha, s, sc);

            local_search(s, sc);

            double cut_value = compute_cut(s, sc, g);

            if (i == 0 or cut_value > best_cut)
            {
                best_cut = cut_value;
                best_s = s;
                best_sc = sc;
            }
        }
        return best_cut;
    }
};

#endif