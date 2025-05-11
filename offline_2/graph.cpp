#ifndef GRAPH_HPP
#define GRAPH_HPP

#include<bits/stdc++.h>
using namespace std;
#define ll long long int

struct edge{
    int u, v;
    ll w;
    edge(int u, int v, ll w) : u(u), v(v), w(w) {}
    bool operator<(const edge& other) const {
        if (w == other.w) {
            if (u == other.u) {
                return v < other.v;
            }
            return u < other.u;
        }
        return w < other.w;
    }
};




#endif