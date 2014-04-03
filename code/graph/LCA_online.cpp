#include "template.cpp"

/*
 * name 	: 	LCA online
 * usage 	:	least common ancestor in tree	
 * develop	:	rmq, lca offline  
 * space complexity	:	O(M)
 * time complexity	:	O(N * logN) - O(logN)
 * checked	:	no
 */

const int N = 10000, M = 20;

int n, m, q;
int tot;
vector<int> E[N];
int dep[N];
int dist[N];
int anc[M][N];
int tin[N], tout[N];
bool vis[N];

void dfs(int x){
	vis[x] = true;
	tin[x] = tot++;
	int y;
	rep(i, E[x].size()){
		y = E[x][i];
		if (vis[y]) continue;
		dep[y] = dep[x] + 1;
		anc[0][y] = x;
		dfs(y);
	}
	tout[x] = tot++;
}

bool isanc(int x, int y){
	return tin[x] <= tin[y] && tout[y] <= tout[x];
}

int lca(int x, int y){
	if (isanc(x, y)) return x;
	fba(j, M - 1, 0){
		if (isanc(anc[j][x], y)) continue;
		x = anc[j][x];
	}
	return anc[0][x];
}

int main(){
	while(~scanf("%d%d", &n, &m)){
		rep(i, n) E[i].clear();
		rep(i, n) vis[i] = false;
		int x, y;
		rep(i, m){
			scanf("%d%d", &x, &y);
			x--, y--;
			E[x].PB(y);
			E[y].PB(x);
		}
		tot = 0;
		dep[0] = 1;
		rep(i, n) anc[i][0] = 0;
		dfs(0);
		REP(j, 1, M) rep(i, n) anc[j][i] = anc[j - 1][anc[j - 1][i]];
		scanf("%d", &q);
		while(q--){
			scanf("%d%d", &x, &y);
			x--, y--;
			printf("%d\n", lca(x, y) + 1);
		}
	}
	return 0;
}
