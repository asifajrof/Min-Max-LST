#include "jngen/jngen.h"
#include <bits/stdc++.h>

using namespace std;

int main(int argc, char* argv[]) {

    int n, m;
    cin>>n>>m;
    cout<<n<<" "<<m<<"\n";
    auto g = Graph::random(n, m).connected().g().shuffled();
    cout << g <<endl;
}
/*

*/
