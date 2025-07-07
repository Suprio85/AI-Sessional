#include<bits/stdc++.h>
using namespace std;

string trim(const string& str) {
    int first_pos = -1;
    int last_pos = -1;
    for(int i=0; i<str.size(); i++){
        if(str[i]!=' '){
            first_pos = i;
            break;
        }
    }
     for(int i=str.size()-1; i>=0; i--){
        if(str[i]!= ' '){
            last_pos = i;
            break;
        }
    }

    return str.substr(first_pos,last_pos-first_pos+1);
}



int main(){

    string s;
    getline(cin,s);

    cout <<" trimed string: " << trim(s) << endl;
    cout <<"length of trimmed string: "<<trim(s).size() << endl;

}