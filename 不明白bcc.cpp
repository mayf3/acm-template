//By myf
#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <set>
#include <map>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <cmath>
#include <cstdlib>
#include <bitset>
#include <complex>

#define rep(i,n) for(int i=0;i<(n);i++)
#define REP(i,l,r) for(int i=(l);i<(r);i++)
#define fab(i,a,b) for(int i=(a);i<=(b);i++)
#define fba(i,b,a) for(int i=(b);i>=(a);i--)
#define foreach(i,n) for(__typeof(n.begin()) i=n.begin();i!=n.end();i++)
#define MP make_pair
#define PB push_back
#define Cls(x) memset(x,0,sizeof x)
#define Print(n,x) for(int i=0;i<(n);i++) cout<<x<<' ';cout<<endl;
#define X first
#define Y second

using namespace std;

typedef long long LL;
typedef pair<int,int> PII;
typedef pair<pair<int,int>,int> PIII;
typedef vector<int> VI;

const int N=10000;

int n,m,tot;
int low[N],dep[N],vis[N];
VI E[N];
bool cut[N];

void dfs(int x,int f,int d){
    int e=0,deg=0,v,z;
    low[x]=dep[x]=d;
    vis[x]=true;
    rep(i,E[x].size()){
        int y=E[x][i];
        if (!vis[y]){
            deg++;
            dfs(y,x,d+1);
            low[x]=min(low[x],low[y]);
            if (low[y]>dep[x]) cut[x]|=(x==root&&deg>1||x!=root&&low[y]>=dep[x]);
        }
        else if (y!=f&&e) low[x]=min(low[x],dep[y]);
        else e=1;
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
        Cls(vis);
        Cls(cut);
        dfs(root=0,-1,0);
        tot=0;
        Cls(vis);
        rep(i,n) if (vis[i]==0) make(i,tot++);
    }
    return 0;
}
