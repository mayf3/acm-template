#include <cstdio>
#include <cstring>
#include <algorithm>

using namespace std;

const int maxn = 200 + 10;
const int maxm = maxn * maxn;
const int INF = 100000000;

struct Tedge {
	int v, f, c, next;
};

struct Ttree {
	int u, v, w;
};

Tedge edge[maxm];
int first[maxn], level[maxn], nedge[maxn], pedge[maxn], prev[maxn], queue[maxn], ans[maxn];
Ttree tree[maxn];
int tree0[maxn];
int a[maxn][maxn], b[maxn][maxn];
bool cut[maxn][maxn];
bool used[maxn];
int n, m, S, T;

inline void add_edge(int u, int v, int c1, int c2 = 0) {
	edge[m].v = v; edge[m].f = 0; edge[m].c = c1; edge[m].next = first[u]; first[u] = m++;
	edge[m].v = u; edge[m].f = 0; edge[m].c = c2; edge[m].next = first[v]; first[v] = m++;
}

int locate(int p) {
	for (int i = 0; i < p - 1; ++i) tree0[i] = i;
	for (int i = 0; i < p; ++i) queue[i] = i;
	for (int t = p; t > 1; ) {
		int k = tree0[0];
		for (int i = 1; i < t - 1; ++i)
			if (tree[tree0[i]].w < tree[k].w) k = tree0[i];
		int tmp = 0;
		for (int i = 0; i < t - 1; ++i)
			if (cut[k][tree[tree0[i]].u] == cut[k][p] &&
				cut[k][tree[tree0[i]].v] == cut[k][p]) tree0[tmp++] = tree0[i];
		tmp = 0;
		for (int i = 0; i < t; ++i)
			if (cut[k][queue[i]] == cut[k][p]) queue[tmp++] = queue[i];
		t = tmp;
	}
	return queue[0];
}

bool newphase() {
	for (int i = 0; i < n; ++i) level[i] = n, nedge[i] = first[i];
	queue[0] = S; level[S] = 0;
	for (int h = 0, t = 1; h < t; ++h) {
		int u = queue[h];
		for (int i = first[u]; i != -1; i = edge[i].next)
			if (edge[i].f < edge[i].c && level[edge[i].v] == n) {
				level[edge[i].v] = level[u] + 1;
				if (edge[i].v == T) return 1;
				queue[t++] = edge[i].v;
			}
	}
	return 0;
}

bool find_path(int u) {
	for (int i = nedge[u]; i != -1; i = edge[i].next)
		if (edge[i].f < edge[i].c && level[edge[i].v] == level[u] + 1)
			if (edge[i].v == T || find_path(edge[i].v)) {
				pedge[edge[i].v] = nedge[u] = i;
				return 1;
			}
	nedge[u] = -1;
	return 0;
}

int Dinic() {
	for (int i = 0; i < m; ++i) edge[i].f = 0;
	int ret = 0;
	while (newphase())
		while (find_path(S)) {
			int delta = INF;
			for (int u = T, i = pedge[u]; u != S; u = edge[i ^ 1].v, i = pedge[u])
				delta = min(delta, edge[i].c - edge[i].f);
			for (int u = T, i = pedge[u]; u != S; u = edge[i ^ 1].v, i = pedge[u])
				edge[i].f += delta, edge[i ^ 1].f -= delta;
			ret += delta;
		}
	return ret;
}

void DFS(int i) {
	used[i] = 1;
	for (int j = 0; j < n; ++j)
		if (b[i][j] && !used[j]) {
			ans[j] = min(ans[i], b[i][j]);
			DFS(j);
		}
}

int main() {
	int N;
	scanf("%d", &N);
	for (int tst = 1; tst <= N; ++tst) {
		m = 0;
		memset(first, -1, sizeof(first));
		scanf("%d", &n);
		for (int i = 0; i < n; ++i)
			for (int j = 0; j < n; ++j) {
				scanf("%d", &a[i][j]);
				if (i < j && a[i][j]) add_edge(i, j, a[i][j], a[i][j]);
			}
	
		memset(cut, 0, sizeof(cut));
		memset(b, 0, sizeof(b));
		for (S = 1; S < n; ++S) {
			T = locate(S);
			tree[S - 1].u = S; tree[S - 1].v = T; tree[S - 1].w = b[S][T] = b[T][S] = Dinic();
			for (int i = 0; i < n; ++i)
				if (level[i] != n) cut[S - 1][i] = 1;
		}

		printf("Case #%d:\n", tst);
		for (int i = 0; i < n; ++i) {
			memset(used, 0, sizeof(used));
			memset(ans, 0, sizeof(ans));
			ans[i] = INF;
			DFS(i);
			ans[i] = 0;
			for (int j = 0; j < n; ++j) {
				if (j) printf(" ");
				printf("%d", ans[j]);
			}
			printf("\n");
		}
	}

	return 0;
}
