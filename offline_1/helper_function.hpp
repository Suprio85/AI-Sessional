#ifndef HELPER_FUNCTION_HPP
#define HELPER_FUNCTION_HPP

#include<bits/stdc++.h>
using namespace std;

vector<vector<int>> getTargetGrid(int n)
{
    vector<vector<int>> grid(n,vector<int>(n,0));
    int counter=1;
    for(int i=0; i<n; i++)
    {
        for(int j=0; j<n; j++)
        {
            if(i==n-1 && j==n-1)
            {
                grid[i][j]=0;
            }
            else
            {
                grid[i][j]=counter++;
            }
        }
    }
    return grid;
}

vector<int> Flattern(vector<vector<int>> grid)
{
    vector<int> flat;
    for(int i=0; i<grid.size(); i++)
    {
        for(int j=0; j<grid[i].size(); j++)
        {
            if(grid[i][j]==0) continue;     /// ignores blank
            flat.push_back(grid[i][j]);
        }
    }
    return flat;
}


int find_row_of_blank_from_bottom(vector<vector<int>> grid){
    int k = grid[0].size();


    for(int i= k-1; i>=0; i--){
        for(int j = k-1; j>=0; j--){
            if(grid[i][j]==0)  return k-i;  /// k+1 worked?
        }
    }
    return -1;
}

int count_inversions(vector<vector<int>> grid){
    vector<int> grid_1D = Flattern(grid);
    int n = grid_1D.size();
    int inversion = 0;

    for(int i = 0; i<n-1; i++){
        for(int j=i+1; j<n;j++){
            inversion+=(grid_1D[i]>grid_1D[j]);
        }
    }

    return inversion;
}


#endif