#ifndef GRAPH_HPP
#define GRAPH_HPP

#include<bits/stdc++.h>
using namespace std;

struct edge{
    int u, v, w;
    edge(int u, int v, int w) : u(u), v(v), w(w) {}
    bool operator<(const edge& other) const {
        return w < other.w;
    }
};

struct graph{
    int n,m;
    vector<edge> edges;
    vector<vector<pair<int,int>>> adj; 
};


#endif