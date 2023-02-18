#include<bits/stdc++.h>
using namespace std;
#define DBG(a) cerr<< "line "<<__LINE__ <<" : "<< #a <<" --> "<<(a)<<endl


struct Graph{
    // declare 5 sets IN, BN, LN, FL, FREE
    set<int>IN, BN, LN, FL, FREE;

    void reset(){
        IN.clear();
        BN.clear();
        LN.clear();
        FL.clear();
        FREE.clear();
    }

    int n;
    void init(int N){
        n = N;
        reset();
        adj.resize(n);
        for(int i=1; i<=n; ++i){
            FREE.insert(i);
        }
    }

    vector< set<int> >adj;
    // create a function to add edge
    void addEdge(int u, int v){
        adj[u].insert(v);
        adj[v].insert(u);
    }
    // create a function to remove edge
    void removeEdge(int u, int v){
        adj[u].erase(v);
        adj[v].erase(u);
    }
    // create a function to check if edge exists
    bool isEdge(int u, int v){
        return adj[u].find(v) != adj[u].end();
    }

    vector<int>intime;
    vector<int>lowtime;
    vector<bool>is_cut;
    int timer = 0;
    void dfs(int u, int p=-1){
        intime[u] = lowtime[u] = timer++;
        int children = 0;
        for(int v: adj[u]){
            if(v == p) continue;
            if(intime[v] == -1){
                dfs(v, u);
                lowtime[u] = min(lowtime[u], lowtime[v]);
                if(lowtime[v] >= intime[u] and p != -1){
                    is_cut[u] = true;
                }
                ++children;
            }
            else{
                lowtime[u] = min(lowtime[u], intime[v]);
            }
        }
        if(p == -1 && children > 1)
            is_cut[u] = true;
    }

    void find_cut_vertex(){
        timer = 0;
        intime.assign(n, -1);
        lowtime.assign(n, -1);
        is_cut.assign(n, false);
        for(int i=0; i<n; ++i){
            if(intime[i] == -1){
                dfs(i);
            }
        }
    }

    bool find_reachability(int s){
        queue<int>q;
        q.push(s);
        vector<bool>vis(n, false);
        vis[s] = true;
        while(!q.empty()){
            int u = q.front();
            q.pop();
            for(int v: adj[u]){
                if(!vis[v]){
                    vis[v] = true;
                    q.push(v);
                }
            }
        }
        if(accumulate(vis.begin(), vis.end(), 0) == n)
            return true;
        return false;
    }

    int deg(int u){
        if(BN.find(u) != BN.end()) {
            int ret = 0;
            for(int v : adj[u]){
                if(FREE.find(v) != FREE.end() or FL.find(v) != FL.end()) ++ret;
            }
            return ret;
        }
        else if(FL.find(u) != FL.end()) {
            int ret = 0;
            for(int v : adj[u]){
                if(FREE.find(v) != FREE.end() or BN.find(v) != BN.end()) ++ret;
            }
            return ret;
        }
        else if(FREE.find(u) != FREE.end()) {
            int ret = 0;
            for(int v : adj[u]){
                if(FREE.find(v) != FREE.end() or FL.find(v) != FL.end() or BN.find(v) != BN.end() ) ++ret;
            }
            return ret;
        }
        else assert(false);
    }

    bool R1(){
        bool ruleApplied = false;
        for(int u : FL){
            for(int v : FL){
                if(u == v) continue;
                if(isEdge(u, v)){
                    removeEdge(u, v);
                    ruleApplied = true;
                }
            }
        }
        for(int u : BN){
            for(int v : BN){
                if(u == v) continue;
                if(isEdge(u, v)){
                    removeEdge(u, v);
                    ruleApplied = true;
                }
            }
        }
        return ruleApplied;
    }

    bool R2(){
        for(int v : BN){
            if( deg(v) == 0){
                BN.erase(v);
                LN.insert(v);
                return true;
            }
        }
        return false;
    }

    bool R3(){
        for(int v : FREE){
            if( deg(v) == 1 ){
                FREE.erase(v);
                FL.insert(v);
                return true;
            }
        }
        return false;
    }

    bool R4(){
        for(int u : FREE){
            int cnt = 0;
            for(int v : adj[u]){
                if(FREE.find(v)==FREE.end() and FL.find(v)==FL.end()) continue;
                else ++cnt;
            }
            if(cnt == 0){
                FL.insert(u);
                FREE.erase(u);
                return true;
            }
        }
        return false;
    }

    bool R5(){
        for(int u : FREE){
            if(deg(u) == 2){
                // cast set to vector
                vector<int>neighbors(adj[u].begin(), adj[u].end());
                for(int i=0; i<(int)neighbors.size(); ++i){
                    for(int j=i+1; j<(int)neighbors.size(); ++j){
                        int v = neighbors[i];
                        int w = neighbors[j];
                        if(isEdge(v, w)){
                            FREE.erase(u);
                            FL.insert(u);
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }


    bool R6(){
        find_cut_vertex();
//        printAdj();
        for(int u : BN){
            if(is_cut[u]){
                x2IN(u);
                return true;
            }
        }
        return false;
    }

    bool R7(){
        for(int u : LN){
            for(int v : adj[u]){
                if(IN.find(v) == IN.end()) {
                    removeEdge(u, v);
                    return true;
                }
            }
        }
        return false;
    }

    void x2IN(int x){
        if(BN.find(x) != BN.end())
            BN.erase(x);
        else if(FL.find(x) != FL.end())
            FL.erase(x);
        else if(FREE.find(x) != FREE.end())
            FREE.erase(x);
        else assert(false);

        IN.insert(x);
        for(int v : adj[x]){
            if(FREE.find(v) != FREE.end()){
                FREE.erase(v);
                BN.insert(v);
            }
            if(FL.find(v) != FL.end()){
                FL.erase(v);
                LN.insert(v);
            }
        }
    }

    void reduction() {
        bool ruleApplied = true;
        while (ruleApplied) {
            ruleApplied = false;
            while (R1()){
                ruleApplied = true;
//                cout << "R1" << endl;
                printPartition();
            }
            while (R2()){
                ruleApplied = true;
//                cout << "R2" << endl;
                printPartition();
            }
            while (R3()){
                ruleApplied = true;
//                cout << "R3" << endl;
                printPartition();
            }
            while (R4()){
                ruleApplied = true;
//                cout << "R4" << endl;
                printPartition();
            }
            while (R5()){
                ruleApplied = true;
//                cout << "R5" << endl;
                printPartition();
            }
            while (R6()){
                ruleApplied = true;
//                cout << "R6" << endl;
                printPartition();
            }
            while (R7()){
                ruleApplied = true;
//                cout << "R7" << endl;
                printPartition();
            }
        }
    }

    void printAdj(){
        for(int i=0; i<n; i++){
            cout<<i<<" : ";
            for(int v : adj[i]){
                cout<<v<<" ";
            }
            cout<<endl;
        }
    }
    void printPartition(){
        return;
        cout<<"IN = "; for(int x : IN) cout<<x<<" "; cout<<endl;
        cout<<"FL = "; for(int x : FL) cout<<x<<" "; cout<<endl;
        cout<<"FREE = "; for(int x : FREE) cout<<x<<" "; cout<<endl;
        cout<<"BN = "; for(int x : BN) cout<<x<<" "; cout<<endl;
        cout<<"LN = "; for(int x : LN) cout<<x<<" "; cout<<endl;
        cout<<endl;
    }

};

int AlgorithmM(int rut, Graph G){

//    cout<<" dfs at here rut = "<<rut<<endl;
    G.printPartition();

    G.reduction();

//    cout<<" afgter reduction rut = "<<rut<<endl;
    G.printPartition();

    if(!G.find_reachability(rut))
        return 0;
    if((int)G.IN.size() + (int)G.LN.size() == G.n)
        return (int)G.LN.size();



    int v = -1, mxdeg = -1;
    for(int u : G.BN){
        int D = G.deg(u);
        if(D > mxdeg){
            mxdeg = D;
            v = u;
        }
    }

    auto hasFLneighbor = [&](int u){
        for(int w : G.adj[u]){
            if(G.FL.find(w) != G.FL.end()) return true;
        }
        return false;
    };

    if(mxdeg >= 3 or (mxdeg==2 and hasFLneighbor(v))){
        Graph G2 = G;
        G2.x2IN(v);
        G.BN.erase(v);
        G.LN.insert(v);
        return max(AlgorithmM(rut, G), AlgorithmM(rut, G2));
    }
    else if(mxdeg == 2){
        // get intersection of adj[v] and FREE
        vector<int>neighbors(G.adj[v].begin(), G.adj[v].end());
        neighbors.erase(remove_if(neighbors.begin(), neighbors.end(), [&](int u){
            return G.FREE.find(u) == G.FREE.end();
        }), neighbors.end());
        assert(neighbors.size() == 2);
        int dg1 = G.deg(neighbors[0]), dg2 = G.deg(neighbors[1]);
        if(dg1 > dg2) swap(neighbors[0], neighbors[1]), swap(dg1, dg2);
        int x1 = neighbors[0], x2 = neighbors[1];

        if(dg1==2){
            vector<int>Nx1(G.adj[x1].begin(), G.adj[x1].end());
            Nx1.erase(find(Nx1.begin(), Nx1.end(), v));
            assert(Nx1.size() == 1);
            int z = Nx1[0];
            if(G.FREE.find(z) != G.FREE.end()){
                Graph G2 = G, G3 = G;
                G.BN.erase(v); G.LN.insert(v);
                G2.x2IN(v); G2.x2IN(x1);
                G3.x2IN(v); G3.LN.insert(x1), G3.FREE.erase(x1);
                return max(max(AlgorithmM(rut, G2), AlgorithmM(rut, G3)), AlgorithmM(rut, G));
            }
            else if(G.FL.find(z) != G.FL.end()){
                G.x2IN(v);
                return AlgorithmM(rut, G);
            }
            else return 0;
        }
        else {
            // cast adj[ x1 ] to set
            set<int> Nx1x2(G.adj[x1].begin(), G.adj[x1].end());
            // insert adj[ x2 ] to set
            Nx1x2.insert(G.adj[x2].begin(), G.adj[x2].end());
            // erase G.FL from set
            set<int> Nx1x2FL;
            set<int>Nx1x2minusFL = Nx1x2;
            for(int u : G.FL){
                if(Nx1x2.find(u) != Nx1x2.end()) Nx1x2FL.insert(u);
                Nx1x2minusFL.erase(u);
            }
            bool ok1 = (int)Nx1x2minusFL.size() == 1 and *Nx1x2minusFL.begin() == v;

            bool ok2 = true;
            for (int z : Nx1x2FL) {
                if (G.deg(z) < 3) {
                    ok2 = false;
                    break;
                }
            }
            if(ok1 and ok2){
                Graph G2 = G, G3 = G, G4 = G;

                G.BN.erase(v); G.LN.insert(v);

                G2.x2IN(v); G2.x2IN(x1);

                G3.x2IN(v); G3.x2IN(x2); G3.FREE.erase(x1); G3.LN.insert(x1);

                G4.x2IN(v); G3.FREE.erase(x1); G3.LN.insert(x1); G3.FREE.erase(x2); G3.LN.insert(x2);
                // get intersection of FREE with adj[ x1 ]
                set<int>Nx1FREE, Nx2FREE;
                for(int u : G.adj[x1]){
                    if(G.FREE.find(u) != G.FREE.end()) Nx1FREE.insert(u);
                }
                for(int u : G.adj[x2]){
                    if(G.FREE.find(u) != G.FREE.end()) Nx2FREE.insert(u);
                }
                // find unioin of Nx1FREE and Nx2FREE
                set<int> Nx1x2FREE;
                set_union(Nx1FREE.begin(), Nx1FREE.end(), Nx2FREE.begin(), Nx2FREE.end(), inserter(Nx1x2FREE, Nx1x2FREE.begin()));

                set<int>Nx1BN, Nx2BN;
                for(int u : G.adj[x1]){
                    if(G.BN.find(u) != G.BN.end()) Nx1BN.insert(u);
                }
                for(int u : G.adj[x2]){
                    if(G.BN.find(u) != G.BN.end()) Nx2BN.insert(u);
                }
                // find unioin of Nx1BN and Nx2BN
                set<int> Nx1x2BN;
                set_union(Nx1BN.begin(), Nx1BN.end(), Nx2BN.begin(), Nx2BN.end(), inserter(Nx1x2BN, Nx1x2BN.begin()));
                Nx1x2BN.erase(v);
                for(int u : Nx1x2FREE){
                    G4.FREE.erase(u);
                    G4.FL.insert(u);
                }
                for(int u : Nx1x2BN){
                    G4.BN.erase(u);
                    G4.LN.insert(u);
                }
                return max(max(max(AlgorithmM(rut, G2), AlgorithmM(rut, G3)), AlgorithmM(rut, G4)), AlgorithmM(rut, G));
            }
            else{
                Graph G2 = G, G3 = G;
                G.BN.erase(v); G.LN.insert(v);
                G2.x2IN(v); G2.x2IN(x1);
                G3.x2IN(v); G3.FREE.erase(x1); G3.LN.insert(x1); G3.x2IN(x2);
                return max(max(AlgorithmM(rut, G2), AlgorithmM(rut, G3)), AlgorithmM(rut, G));
            }
        }
    }
    else if(mxdeg == 1){
        vector<int>P = {v};
        while (true){
//            cout<<"stuck here "<<endl;
            bool found = false;
            for(int u : G.adj[ P.back() ]){
                if(P.size()>=2 and P[P.size()-2] == u) continue;
                if(G.FREE.find(u) != G.FREE.end()){
                    if(G.deg(u)==2){
                        P.push_back(u);
                        found = true;
                        break;
                    }
                }
            }
            if(!found) break;
        }
        set<int>Nvk = G.adj[ P.back() ];
        for(int u : P) Nvk.erase(u);
        // assert((int)Nvk.size() == 1);
        int z = -1;
        for(int u : Nvk) {
            if(G.IN.find(u) == G.IN.end()) z = u;
        }
        if(z==-1){
            return 0;
        }
        int zdeg = G.deg(z);
        if(G.FL.find(z)!=G.FL.end() and zdeg==1){
            for(int w : P) G.x2IN(w);
            G.FL.erase(z);  G.LN.insert(z);
            return AlgorithmM(rut, G);
        }
        else if(G.FL.find(z) != G.FL.end() and zdeg>1){
            int vk = P.back(); P.pop_back();
            for(int w : P) G.x2IN(w);
            G.FREE.erase(vk);  G.LN.insert(vk);
            return AlgorithmM(rut, G);
        }
        else if(G.BN.find(z) != G.BN.end() ){
            G.BN.erase(v); G.LN.insert(v);
            return AlgorithmM(rut, G);
        }
        else if(G.FREE.find(z) != G.FREE.end() ){
            Graph G2 = G;
            for(int w : P) G.x2IN(w);
            G.x2IN(z);
            G2.BN.erase(v); G2.LN.insert(v);
            return max(AlgorithmM(rut, G2), AlgorithmM(rut, G));
        }
        else return 0;
    }
    else return 0;
}

void solv(Graph &G){
    int ans = -1;
    for(int i=0; i<G.n; i++){
//        cout<<"setting root at "<<i<<endl;
        G.reset();
        for(int j=0; j<G.n; j++) G.FREE.insert(j);
        G.IN.insert(i), G.FREE.erase(i);
        for(int v : G.adj[i]){
            G.BN.insert(v), G.FREE.erase(v);
        }
        ans = max(ans, AlgorithmM(i, G));
    }
    cout<<ans<<endl;
}

int main(){
    ios::sync_with_stdio(false); cin.tie(0);

    int n,m;
    cin>>n>>m;
    Graph G;
    G.init(n);
    for(int i=0; i<m; ++i){
        int u, v;
        cin>>u>>v;
        G.addEdge(u, v);
    }
    solv(G);
}
