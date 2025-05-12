#include<bits/stdc++.h>
#include "2105085_graph.cpp"
using namespace std;

#define ll long long int

#define INF INT_MAX
#define MAXN 5005

double random_choice_prob()
{
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);

    return dis(gen);
}

ll compute_cut(const bitset<MAXN>& x, const bitset<MAXN>& y, const vector<edge>& edges) {
    ll cut = 0;
    for (auto edge : edges) {
        if (x[edge.u] && y[edge.v]) {
            cut += edge.w;
        }
        else if (y[edge.u] && x[edge.v]) {
            cut += edge.w;
        }
    }
    return cut;
}

pair<ll,ll> compute_sigma(const bitset<MAXN> &X, const bitset<MAXN> &Y, const vector<vector<pair<ll,int>>> &adj, int v) {
    ll sigma_x = 0, sigma_y = 0;
    for (auto [w,u] : adj[v]) {
        if (Y[u]) {
            sigma_x += w;
        }
        else if (X[u]) {
            sigma_y += w;
        }
    }
    return {sigma_x, sigma_y};
}

ll compute_weight(int z, const bitset<MAXN> &s, const vector<vector<pair<ll,int>>> &adj) {
    ll weight = 0.0;
    for (const auto &[w, v] : adj[z])
    {
        if (s[v])
            weight += w;
    }

    return weight;
}


double random_heuristic(int num_ieterations, const vector<edge>& edges, const vector<vector<pair<ll,int>>>& adj, vector<int>& x, vector<int>& y)
{
    ll total_cut_weight = 0;
    int n = adj.size()-1;
    
    for (int i = 0; i < num_ieterations; i++)
    {
        x.clear();
        y.clear();
        bitset<MAXN> X, Y;
        X.reset();
        Y.reset();
        
        for (int v = 1; v <= n; v++)
        {
            auto random_choice_w_prob = random_choice_prob();
            if (random_choice_w_prob >= 0.5){
                x.push_back(v);
                X.set(v);
            }
            else{
                y.push_back(v);
                Y.set(v);
            }
        }
        total_cut_weight += compute_cut(X, Y, edges);
    }

    double avg_cut_weight = (1.0 * total_cut_weight) / num_ieterations;
    return avg_cut_weight;
}

ll greedy_heuristic(vector<edge>& edges, const vector<vector<pair<ll,int>>>& adj) 
{
    int n = adj.size()-1;
    int u = edges[0].u;
    int v = edges[0].v;

    bitset<MAXN> X, Y;
    X=0, Y=0;
    X.set(u);
    Y.set(v);

    for (int V = 0; V < n; V++)
    {
        if (V == v or V == u)
            continue;
        auto w_x = compute_weight(V, Y, adj);
        auto w_y = compute_weight(V, X, adj);

        if (w_x > w_y)
            X.set(V);
        else
        {
            Y.set(V);
        }
    }

    // x.clear();
    // y.clear();
    // for (int i = 1; i <= n; i++) {
    //     if (Y[i]) {
    //         y.push_back(i);
    //     }
    //     else if (X[i]) {
    //         x.push_back(i); 
    //     }
    // }

    return compute_cut(X, Y, edges);
}

ll semi_greedy_maxcut(vector<edge>& edges, vector<vector<pair<ll,int>>>& adj, ll alpha, vector<int> &x, vector<int> &y) {
    int n = adj.size()-1;
    bitset<MAXN> X, Y;
    X.reset();
    Y.reset();
    
    ll w_min = edges.back().w;
    ll w_max = edges[0].w;
    ll mu = w_min * 100LL + alpha * (w_max - w_min);
    
    vector<int> v_prime;
    for (int i = 1; i <= n; i++) v_prime.push_back(i);
    
    vector<edge> RCL_edges;
    for (auto edge : edges) {
        if (edge.w * 100LL >= mu) RCL_edges.push_back(edge);
        else break;
    }
    
    int pick_idx = 0;
    if (!RCL_edges.empty()) pick_idx = rand() % RCL_edges.size();
    auto edge = RCL_edges[pick_idx];
    X.set(edge.u);
    Y.set(edge.v);
    
    // Remove edge.u and edge.v from v_prime
    auto it_u = find(v_prime.begin(), v_prime.end(), edge.u);
    if (it_u != v_prime.end()) v_prime.erase(it_u);
    
    auto it_v = find(v_prime.begin(), v_prime.end(), edge.v);
    if (it_v != v_prime.end()) v_prime.erase(it_v);
    
    while (!v_prime.empty()) {
        vector<ll> sigma_x(n+1), sigma_y(n+1);
        ll sigma_x_min = INF, sigma_x_max = -INF;
        ll sigma_y_min = INF, sigma_y_max = -INF;
        for (auto v : v_prime) {
            auto [_x, _y] = compute_sigma(X, Y, adj, v);
            sigma_x[v] = _x;
            sigma_y[v] = _y;
            sigma_x_min = min(sigma_x_min, sigma_x[v]);
            sigma_x_max = max(sigma_x_max, sigma_x[v]);

            sigma_y_min = min(sigma_y_min, sigma_y[v]);
            sigma_y_max = max(sigma_y_max, sigma_y[v]);
        }
        ll w_min = min(sigma_x_min, sigma_y_min);
        ll w_max = max(sigma_x_max, sigma_y_max);
        mu = min(w_max * 100LL, w_min * 100LL + alpha * (w_max - w_min));

        vector<int> RCL_vertices;
        for (auto v : v_prime) {
            if ((max(sigma_x[v], sigma_y[v]) * 100LL) >= mu) {
                RCL_vertices.push_back(v);
            }
        }
        
        int pick_idx = rand() % RCL_vertices.size();
        auto vertex = RCL_vertices[pick_idx];

        if (sigma_x[vertex] > sigma_y[vertex])
            X.set(vertex);
        else
            Y.set(vertex);
      
        auto it = find(v_prime.begin(), v_prime.end(), vertex);
        if (it != v_prime.end()) v_prime.erase(it);
    }

    x.clear();
    y.clear();
    for (int i = 1; i <= n; i++) {
        if (Y[i]) {
            y.push_back(i);
        }
        else if (X[i]) {
            x.push_back(i); 
        }
    }
    ll cut = compute_cut(X, Y, edges);
    return cut;
}

pair<ll,int> local_search(vector<edge>& edges, const vector<vector<pair<ll,int>>>& adj,vector<int> &x, vector<int> &y ) {
    int n = adj.size()-1;
    int num_iterations = 0;
    bitset<MAXN> X, Y;
    X.reset();
    Y.reset();
    
    for (auto v : x) X.set(v);
    for (auto v : y) Y.set(v);

    while (true) {
        num_iterations++;
        double bestDelta = 0;
        int bestV = -1;

        for (int v = 1; v <= n; v++) {
            bool in_x = X[v];
            double sigma_s  = compute_weight(v, Y, adj);
            double sigma_sc = compute_weight(v, X, adj);
            double delta    = in_x ? (sigma_sc - sigma_s) : (sigma_s - sigma_sc);

            if (delta > bestDelta) {
                bestDelta = delta;
                bestV = v;
            }
        }
        if (bestDelta > 0) {
            if (X[bestV]) { X.reset(bestV); Y.set(bestV); }
            else          { Y.reset(bestV); X.set(bestV); }
        } else {
            break;
        }
    }

    x.clear(); y.clear();
    for (int v = 1; v <= n; v++)
    {
        if (X[v]) x.push_back(v);
        else if(Y[v]) y.push_back(v);
    }

    return { compute_cut(X, Y, edges), num_iterations };
}

pair<ll,ll> GRASP(vector<edge>& edges, vector<vector<pair<ll,int>>>& adj, ll alpha, int num_iterations)
{
    int n = adj.size()-1;
    vector<int> x;
    vector<int> y;

    ll best_cut = 0;
    ll iter=0;
    vector<int> best_s;
    vector<int> best_sc;

    for (int i = 0; i < num_iterations; i++)
    {
        vector<int> s;
        vector<int> sc;
        ll cut = 0;
        semi_greedy_maxcut(edges, adj, alpha, s, sc);

       auto [local_cut, local_iter] = local_search(edges, adj, s, sc);
        iter += local_iter;

        cut = local_cut;
        if(i == 0 || cut > best_cut) {
            best_cut = cut;
            best_s = s;
            best_sc = sc;
        }
    }
    return {best_cut, iter/num_iterations};
}