//By myf
//#pragma comment(linker, "/STACK:16777216")  //C++
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <bitset>
#include <complex>
#include <map>
#include <set>
#include <queue>
#include <deque>
#include <stack>
#include <vector>
#include <list>

#define rep(i,n) for(int i=0;i<(n);i++)
#define REP(i,l,r) for(int i=(l);i<(r);i++)
#define fab(i,a,b) for(int i=(a);i<=(b);i++)
#define fba(i,b,a) for(int i=(b);i>=(a);i--)
//#define foreach(i,n) for(__typeof(n.begin()) i=n.begin();i!=n.end();i++) //G++
#define MP make_pair
#define PB push_back
#define X first
#define Y second
#define Cls(a,x) memset(a,x,sizeof a)
#define Print(n,x) for(int i=0;i<(n);i++) cout<<x<<" ";cout<<endl;

using namespace std;

typedef long long LL;
typedef pair<int,int> PII;
typedef pair<PII,int> PIII;
typedef pair<LL,int> PLI;
typedef vector<int> VI;
typedef int T;

const int N = 50, M = 150;

int n, m;
int x[M],y[M];int psz;
int next[N], match[N], v[N];
int f[N], rank[N];
int ans[M];

VI E[N];
deque<int> Q;

inline int find(int p) {return f[p]<0?p:f[p]=find(f[p]);}

void join(int x, int y){
	x = find(x); y = find(y);
	if (x != y) f[x] = y;
}

int lca(int x, int y){
	static int v[N];
	static int stamp = 0;
	++stamp;
	for (;;) {
		if (x >= 0) {
			x = find(x);
			if (v[x] == stamp) return x;
			v[x] = stamp;
			if (match[x] >= 0) x = next[match[x]];
			else x = -1;
		}
		swap(x, y);
	}
}

void group(int a, int p){
	while (a != p) {
		int b = match[a], c = next[b];
		if (find(c) != p) next[c] = b;
		if (v[b] == 2) Q.PB(b),v[b] = 1;
		if (v[c] == 2) Q.PB(c),v[c] = 1;
		join(a, b); join(b, c);
		a = c;
	}
}

void aug(int s){
    Cls(v,0);
    Cls(next,-1);
    Cls(f,-1);
    Q.clear();
    Q.PB(s);
    v[s] = 1;
    while(!Q.empty()&&match[s]==-1){
        int x=Q.front();Q.pop_front();
        rep(i,E[x].size()){
			int y = E[x][i];
			if (match[x] == y || find(x) == find(y) || v[y] == 2) continue;
			if (v[y] == 1) {
				int p = lca(x, y);
				if (find(x) != p) next[x] = y;
				if (find(y) != p) next[y] = x;
				group(x, p);
				group(y, p);
			} else if (match[y] == -1) {
				next[y] = x;
				while (~y) {
					int z = next[y];
					int p = match[z];
					match[y] = z; match[z] = y;
					y = p;
				}
				break;
			} else {
				next[y] = x;
				Q.PB(match[y]);
				v[match[y]]=1;
				v[y] = 2;
			}
		}
	}
}

//work 是删边，实际上带花树就是这两行：Cls(match,-1);rep(i,n) if (match[i]==-1) aug(i);
int work(int k){
	psz = 0;
	rep(i,n) E[i].clear();
	rep(i,m){
		if (x[i] == x[k]) continue;
		if (x[i] == y[k]) continue;
		if (y[i] == x[k]) continue;
		if (y[i] == y[k]) continue;
		E[x[i]].PB(y[i]);
		E[y[i]].PB(x[i]);
	}
	Cls(match,-1);
	rep(i,n) if (match[i]==-1) aug(i);
	int c = 0;
	rep(i,n) if (match[i]!=-1) c++;
	return c/2;
}

int main(){
	while(~scanf("%d%d", &n, &m)){
		rep(i,m) {
		    scanf("%d%d",x+i,y+i);
		    x[i]--,y[i]--;
		}
		x[m]=y[m]=n;
		int s=work(m);
		int tot=0;
		rep(i,m) if (work(i)!=s-1) ans[tot++]=i+1;
		printf("%d\n", tot);
		if (tot) rep(i,tot) printf(i==tot-1?"%d\n":"%d ",ans[i]);
		else puts("");
	}
}
