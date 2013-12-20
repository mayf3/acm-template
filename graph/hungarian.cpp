#include "template.cpp"

/*
 * name 	: 	hungarian	
 * usage 	:	maximum cardinally bipartite matching
 * develop	:	Hopcroft-Karp(faster), Kuhn-Munkres(with weight)
 * space complexity	:	O(M)
 * time complexity	:	O(N * M)
 * checked	:	no
 */

const int N = 100 + 10;

int n, m;
vector<int> E[N];
bool visit[N];
int v[N];

int find(int x) {
	int y;
	rep(i, E[x].size()){
		y = E[x][i];
		if (visit[y]) continue;
		visit[y] = true;
		if (v[y] == -1 || find(v[y])){
			v[y] = x;
			return true;
		}
	}
	return false;
}

int max_match() {
	memset(v, -1, sizeof(v));
	int res = 0;
	rep(i, n){
		Cls(visit);
		if (find(i)) res++;
	}
	return res;
}

int main(){
	while(~scanf("%d%d", &n, &m)){
		rep(i, n) E[i].clear();
		int x, y;
		rep(i, m){
			scanf("%d%d", &x, &y);
			x--, y--;
			E[x].PB(y);
		}
		printf("%d\n", max_match());
	}
	return 0;
}
