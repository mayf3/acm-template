#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <map>
#include <vector>
#include <set>
#include <cstdlib>
#include <queue>
#include <deque>

#define rep(i,n) for(int i=0;i<(n);i++)
#define REP(i,l,r) for(int i=(l);i<(r);i++)
#define fab(i,a,b) for(int i=(a);i<=(b);i++)
#define fba(i,b,a) for(int i=(b);i>=(a);i--)
#define MP make_pair
#define PB push_back
#define X first
#define Y second
#define Cls(x) memset(x,0,sizeof x)
#define Print(n,x) for(int i=0;i<(n);i++) cout<<x<<" ";cout<<endl;
#define PII pair<int,int>
#define Vi vector<int>
#define

using namespace std;

typedef long long LL;

const int N=1111;

int n,m;
int low[N],dep[N];
Vi E[N];


void dfs(int u, int p, int d) {
  int e = 0, deg = 0, v, z;

  vt[u] = 1;
  low[u] = dep[u] = d;
  for (z = hd[u]; z > -1; z = ed[z].p) {
    if (!vt[v = ed[z].v]) {
      deg++;
      dfs(v, u, d + 1);
      low[u] = min(low[u], low[v]);
      if (low[v] > dep[u])
        ed[z].b = ed[z ^ 1].b = 1;
      cut[u] |= (u == root && deg > 1 || u != root && low[v] >= dep[u]);

    } else if (v != p || e) {
      low[u] = min(low[u], dep[v]);

    } else e = 1;
  }
}

void dfs(int x,int f,int d){
    int e=0,deg=0,v,z;
    low[x]=dep[x]=d;
    rep(i,E[x].size()){
        int y=E[x][i];
        if (low[y]==-1){
            deg++;
            dfs(y,x,d+1);
            low[x]=min(low[x],low[y]);
            if (low[y]>dep[x])
            cut[x]|=(x==root&&deg>1||x!=root&&low[y]>=dep[x]);
        }
        else if ()
    }
}

int main(){
    while(~scanf("%d%d",&n,&m)){
        rep(i,n) E[i].clear(),low[i]=dep[i]=-1;
        rep(i,m) {
            int x,y;
            scanf("%d%d",&x,&y);
            x--,y--;
            E[x].PB(y);
            E[y].PB(x);
        }
    }
    return 0;
}
