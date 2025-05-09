#include<bits/stdc++.h>
#include "Graph.hpp"
#include "constructiveAlgorithm.hpp"
using namespace std;


ostream& operator<<(ostream& os, const graph & g) {
    os << "Number of vertices: " << g.n << "\n";
    os << "Number of edges: " << g.m << "\n";
    os << "Edges:\n";
    for (const auto& e : g.edges) {
        os << e.u << " -- " << e.v << " (weight: " << e.w << ")\n";
    }
    return os;
}   


graph read_graph(const string& filename){
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        exit(1);
    }

    graph g;
    file >> g.n >> g.m;
    g.adj.resize(g.n);

    for (int i = 0; i < g.m; ++i) {
        int u, v, w;
        file >> u >> v >> w;
        u--; // 0-based indexing
        v--;  
        g.edges.emplace_back(u, v, w);
        g.adj[u].push_back({w,v});
        g.adj[v].push_back({w,v}); 
    }

    cout<<g;

    sort(g.edges.rbegin(),g.edges.rend());
    return g;
}



int main(){

    ofstream out("out.txt");

    
    string folder ="set1";
    for(int i=11; i<=11; i++){
        string graph_name ="g"+to_string(i);
        string file_name = graph_name+".rud";
        
        graph g = read_graph(string(folder+"/"+file_name));
        out<<g;
        ConstructiveAlgorithm ca(g);
        set<int> x,y;
        
       auto ans = ca.semi_greedy_heuristics(1,x,y);
    //    ca.local_search(x,y);

       out<<ans<<endl;
         ca.local_search(x,y).first;
    }
   
}

