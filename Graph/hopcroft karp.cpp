#include "bits/stdc++.h"
using namespace std;
typedef vector<int> vi;
typedef pair<int,int> ii;
typedef vector<vi> vvi;
#define pb push_back
#define sz(x) (int)x.size()
#define all(x) x.begin(),x.end() 
#define F first
#define S second
#define FOR(i,a,b) for(int i = a; i<=b; ++i)
#define NFOR(i,a,b) for(int i = a; i>=b; --i)
#define fast ios_base::sync_with_stdio(false),cin.tie(0),cout.tie(0)
const int mod = 1e9+7;
const int N = 1e5+10; 
typedef long long LL;

//https://sites.google.com/site/indy256/algo_cpp/hopcroft_karp
//algo taken from above site
const int MAXN1 = 50000;
const int MAXN2 = 50000;
const int MAXM = 150000;

int n1, n2, edges, last[MAXN1], prevv[MAXM], head[MAXM];
int matching[MAXN2], dist[MAXN1], Q[MAXN1];
bool used[MAXN1], vis[MAXN1];
int match[MAXN1];

void init(int _n1, int _n2) {
    n1 = _n1;
    n2 = _n2;
    edges = 0;
    fill(last, last + n1, -1);
}

void addEdge(int u, int v) {
    head[edges] = v;
    prevv[edges] = last[u];
    last[u] = edges++;
}

void bfs() {
    fill(dist, dist + n1, -1);
    int sizeQ = 0;
    for (int u = 0; u < n1; ++u) {
        if (!used[u]) {
            Q[sizeQ++] = u;
            dist[u] = 0;
        }
    }
    for (int i = 0; i < sizeQ; i++) {
        int u1 = Q[i];
        for (int e = last[u1]; e >= 0; e = prevv[e]) {
            int u2 = matching[head[e]];
            if (u2 >= 0 && dist[u2] < 0) {
                dist[u2] = dist[u1] + 1;
                Q[sizeQ++] = u2;
            }
        }
    }
}

bool dfs(int u1) {
    vis[u1] = true;
    for (int e = last[u1]; e >= 0; e = prevv[e]) {
        int v = head[e];
        int u2 = matching[v];
        if (u2 < 0 || !vis[u2] && dist[u2] == dist[u1] + 1 && dfs(u2)) {
            matching[v] = u1;
            used[u1] = true;
            return true;
        }
    }
    return false;
}

int maxMatching() {
    fill(used, used + n1, false);
    fill(matching, matching + n2, -1);
    for (int res = 0;;) {
        bfs();
        fill(vis, vis + n1, false);
        int f = 0;
        for (int u = 0; u < n1; ++u)
            if (!used[u] && dfs(u))
                ++f;
        if (!f)
            return res;
        res += f;
    }
}
ii func(char c,int i,int j){
	if(c == 'R')return {i,j+1};
	if(c == 'L')return {i,j-1};
	if(c == 'U')return {i-1,j};
	if(c == 'D')return {i+1,j};
}
int main(){
  fast;
  int t;cin >> t;
  while(t--){
  	int n;cin >> n;
  	init(n*n,n*n);
  	FOR(i,0,n-1)FOR(j,0,n-1){
  		string s;
  		cin >> s;
  		FOR(k,0,sz(s)-1){
  			ii lol = func(s[k],i,j);
  			if(lol.F < 0 || lol.S < 0 || lol.F >= n || lol.S >=n )continue;
  			addEdge(i*n + j, lol.F*n + lol.S);
  			//pr->AddEdge(i*n+j,n*n + lol.F*n+lol.S,1);
  		}
  	}
  	//FOR(i,0,n*n-1)pr->AddEdge(src,i,1);
  	//FOR(i,n*n,n*n + n*n - 1)pr->AddEdge(i,dest,1);
  	//LL c = pr->GetMaxFlow(src,dest);
  	int c = maxMatching();
  	if(c != n*n){cout << "NO\n";continue;}
  	cout << "YES\n";
  	//FOR(i,0,n*n-1)cout << matching[i] << " ";cout << "\n";
  	FOR(i,0,n*n-1)match[matching[i]] = i;
  	FOR(i,0,n-1){
  		FOR(j,0,n-1){
			ii c = {match[i*n+j]/n,match[i*n+j]%n};
			//cout << i << " " << j << " " << lol.F << " " << lol.S << "\n";
			if(c.F == i + 1)cout << "D ";
			if(c.F == i - 1)cout << "U ";
			if(c.S == j + 1)cout << "R ";
			if(c.S == j - 1)cout << "L ";
			//cout << it.from << " " << it.to-n*n << " " << it.cap << " " << it.flow  << "\n";
  		}cout << "\n";
  	}
  } 
  return 0;
}