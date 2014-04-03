#include "template.cpp"

/*
 * name 	: 	LCA_offline
 * usage 	:	least common ancestor in tree	
 * develop	:	rmq, lca online  
 * space complexity	:	O(M)
 * time complexity	:	O(M)
 * checked	:	no
 */

const int N = 1000, M = 10000; //number of vertices

int n, m, q;
vector<int> E[N];
vector<PII> Q[N];
int ans[N];
int f[N];
bool vis[N];

int find(int x){
	return (f[x] == x) ? f[x] : f[x] = find(f[x]);
}

void dfs(int x) {
	f[x] = x;
	int y;
	rep(i, E[x].size()){
		y = E[x][i];
		if (f[y] != -1) continue;
		dfs(y);
		f[y] = x;
	}
	vis[x] = true;
	int id;
	rep(i, Q[x].size()){
		y = Q[x][i].X, id = Q[x][i].Y;
		if (vis[y]) ans[id] = find(y);
	}
}


int main(){
	while(~scanf("%d", &n)){
		rep(i, n) E[i].clear(), Q[i].clear();
		rep(i, n) f[i] = -1, vis[i] = false;
		int x, y, c;
		m = n - 1;
		rep(i, m){
			scanf("%d%d", &x, &y);
			x--, y--;
			E[x].PB(y);
			E[y].PB(x);
		}
		scanf("%d", &q);
		rep(i, q){
			scanf("%d%d", &x, &y);
			x--, y--;
			Q[x].PB(MP(y, i));
			Q[y].PB(MP(x, i));
		}
		dfs(0);
		rep(i, q) printf("%d\n", ans[i] + 1);
	}
	return 0;
}
