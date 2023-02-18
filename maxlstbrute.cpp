#include<bits/stdc++.h>
using namespace std;
mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

void solv(){

    int n, m;
    cin>>n>>m;
    assert(n!=1);
    vector<vector<int>> adj(n);
    vector<pair<int,int>> edges;
    for(int i=0; i<m; ++i){
        int u, v;
        cin>>u>>v;
        edges.emplace_back(u, v);
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    for(int i=0; i<n; ++i) {
        shuffle(adj[i].begin(), adj[i].end(), rng);
    }

    for(int leafs = n-1; leafs >= 2; leafs--){
        vector<bool>mask(n), visited(n);
        for(int i=0; i<leafs; ++i) mask[i] = 1;
        do{
            fill(visited.begin(), visited.end(), false);
            for(int i=0; i<n; ++i){
                if(mask[i]){
                    for(int v : adj[i]) if(!mask[v])
                        visited[i] = true;
                }
            }

            if(visited != mask) continue;

            auto dfs = [&] (int u, int p, auto &&dfs) -> void {
                visited[u] = true;
                for(int v : adj[u]) if(v!=p)
                    if(!visited[v] and !mask[v]) dfs(v, u, dfs);
            };

            fill(visited.begin(), visited.end(), false);
            for(int i=0; i<n; ++i){
                if(!mask[i]){
                    dfs(i, -1, dfs);
                    break;
                }
            }
            bool ok = true;
            for(int i=0; i<n and ok; i++) {
                if(mask[i] == visited[i]) ok = false;
            }
            if(ok){
//                cout<<"found "<<leafs<<" leafs"<<endl;
//                for(int i=0; i<n; ++i) if(mask[i]) cout<<i<<" ";
//                cout<<endl;
                cout<<leafs<<"\n";
                return;
            }
        }while(prev_permutation(mask.begin(), mask.end()));
    }
    assert(false);
}
/*
4 5
0 1 0 2 0 3
1 2 2 3
*/

int main(){
    ios::sync_with_stdio(false); cin.tie(0);

    solv();
}
