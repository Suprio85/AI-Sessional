#ifndef HEURISTICS_HPP
#define HEURISTICS_HPP

#include<bits/stdc++.h>
#include "helper_function.hpp"
using namespace std;

int hammingDistance(const vector<vector<int>>& grid)
{
    int distance = 0;
    int k = grid[0].size();

    for(int i=0; i<k; i++){
        for(int j=0; j<k; j++){
            if(grid[i][j]==0) continue;
            int number = grid[i][j];
            int target_x = (number-1)/k;
            int target_y = (number-1)%k;
            distance += !((i==target_x) and (j == target_y));
        }
    }
    return distance;
}

int manhattanDistance(const vector<vector<int>>& grid)
{
    int distance = 0;
    int k = grid[0].size();

    for(int i=0; i<k; i++){
        for(int j=0; j<k; j++){
            if(grid[i][j]==0) continue;
            int target_x = (grid[i][j]-1)/k;
            int target_y = (grid[i][j]-1)%k;
            distance+=abs(i-target_x)+abs(j-target_y);
        }
    }
    return distance;
}

double euclideanDistance(const vector<vector<int>>& grid)
{
    double distance = 0.0;
    int k = grid[0].size();

    for(int i=0; i<k; i++){
        for(int j=0; j<k; j++){
            if(grid[i][j]==0) continue;
            int target_x = (grid[i][j]-1)/k;
            int target_y = (grid[i][j]-1)%k;
            distance+=sqrt(pow(i-target_x,2)+pow(j-target_y,2));
        }
    }
    return distance;
}

int countConflict(const vector<vector<int>>& grid)
{
    int conflict = 0;
    int k = grid[0].size();

    for(int i=0; i<k; i++){
        for(int j=0; j<k; j++){
            if(grid[i][j]==0) continue;
            int target_x = (grid[i][j]-1)/k;
            int target_y = (grid[i][j]-1)%k;
            if(target_x==i){
                for(int j2=j+1; j2<k; j2++){
                    if(grid[i][j2]==0) continue;
                    int target_x2 = (grid[i][j2]-1)/k;
                    int target_y2 = (grid[i][j2]-1)%k;
                    if(target_x2==i && target_y>target_y2){
                        conflict++;
                    }
                }
            }
            else if(target_y==j){
                for(int i2=i+1; i2<k; i2++){
                    if(grid[i2][j]==0) continue;
                    int target_x2 = (grid[i2][j]-1)/k;
                    int target_y2 = (grid[i2][j]-1)%k;
                    if(target_x==target_x2 && target_y>target_y2){
                        conflict++;
                    }
                }
            }
        }
    }

    return conflict;
    
}

int linearConflict(const vector<vector<int>>& grid){
    return manhattanDistance(grid) + 2*countConflict(grid);
}


#endif



