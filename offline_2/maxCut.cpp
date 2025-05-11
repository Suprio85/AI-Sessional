#include <bits/stdc++.h>
#include "Algorithm.cpp"
using namespace std;

#define ll long long int


map<string, ll> known_best = {
    {"G1", 12078}, {"G2", 12084}, {"G3", 12077}, {"G11", 627}, {"G12", 621}, {"G13", 645}, {"G14", 3187}, {"G15", 3169}, {"G16", 3172}, {"G22", 14123}, {"G23", 14129}, {"G24", 14131}, {"G32", 1560}, {"G33", 1537}, {"G34", 1541}, {"G35", 8000}, {"G36", 7996}, {"G37", 8009}, {"G43", 7027}, {"G44", 7022}, {"G45", 7020}, {"G48", 6000}, {"G49", 6000}, {"G50", 5988}};

int main(int argc, char *argv[])
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    
    if (argc < 3 || argc > 5)
    {
        cout << "Usage: " << argv[0] << " start_graph end_graph [alpha=.50] [iterations=50]" << endl;
        return 1;
    }
    int start = stoi(argv[1]);
    int end = stoi(argv[2]);
    if (start < 1 || end > 54 || start > end)
    {
        cout << "Invalid range. Please provide a range between 1 and 54." << endl;
        return 1;
    }
    ll alpha = (argc >= 4) ? stof(argv[3]) * 100ll : 50; // Default alpha = 50
    int iterations = (argc >= 5) ? stoi(argv[4]) : 50;

 
    ofstream csv("2105085_" + to_string(start) + "_" + to_string(end) + ".csv");
    csv << "Problem Name,|V| or n,|E| or m,Simple Randomized or Randomized-1,"
        << "Simple Greedy or Greedy-1,Semi-greedy or Greedy-1,"
        << "Simple local or local-1,No. of iterations,GRASP-1,No. of iterations,"
        << "Known best solution or upper bound\n";

    string folder = "set1";
    for (int i = start; i <= end; i++)
    {
        string graph_name = "G" + to_string(i);
        string file_name = "g" + to_string(i) + ".rud";
        string full_path = folder + "/" + file_name;

        ifstream fin(full_path);
        if (!fin.is_open())
        {
            cout << "Error opening file: " << full_path << endl;
            continue;
        }

        int n, m;
        fin >> n >> m;
        vector<edge> edges;
        vector<vector<pair<ll, int>>> adj(n + 1);
        for (int j = 0; j < m; j++)
        {
            int u, v;
            ll w;
            fin >> u >> v >> w;
            edges.emplace_back(u, v, w);
            adj[u].push_back({w, v});
            adj[v].push_back({w, u});
        }
        sort(edges.rbegin(), edges.rend()); 
        vector<int> S, Sc;

        
        double random_cut = random_heuristic(10, edges, adj, S, Sc);

        
        ll greedy_cut = greedy_heuristic(edges, adj);

        
        ll semi_greedy_cut = semi_greedy_maxcut(edges, adj, alpha, S, Sc);

        int ls_runs = 5;
        ll total_ls_cut = 0;
        int total_ls_iter = 0;
        for (int run = 0; run < ls_runs; run++)
        {
            vector<int> S, Sc;
            semi_greedy_maxcut(edges, adj, 30, S, Sc); 
            auto [ls_cut, ls_iter] = local_search(edges, adj, S, Sc);
            total_ls_cut += ls_cut;
            total_ls_iter += ls_iter;
        }
        ll avg_ls_cut = (double)total_ls_cut / ls_runs;
        ll avg_ls_iter = (double)total_ls_iter / ls_runs;

       
        auto [grasp_cut, grasp_iter] = GRASP(edges, adj, alpha, iterations);

        
        string known = known_best.count(graph_name) ? to_string(known_best[graph_name]) : "N/A";

        csv << graph_name << "," << n << "," << m << "," << fixed << setprecision(2) << random_cut << ","
            << greedy_cut << "," << semi_greedy_cut << " (alpha=" << (alpha / 100.0) << ")," << avg_ls_cut << ","
            << avg_ls_iter << "," << grasp_cut << "," << grasp_iter << "," << known << "\n";
    }

    csv.close();
    cout << "CSV file '2105085.csv' generated successfully." << endl;
    return 0;
}