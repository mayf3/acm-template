const int MAXN = 10000 + 10; //number of vertices
const int MAXDEPTH = 14 + 5; //Log2(MAXN)

struct Tedge {
	int v, next;
};

Tedge edge[MAXN * 2];
int first[MAXN], stack[MAXN], depth[MAXN], dist[MAXN], Log[MAXN];
int anc[MAXN][MAXDEPTH];
int N;

void DFS(int u, int d) {
	depth[u] = d; stack[d] = u;

	for (int i = 1, j = 0; i <= d; i <<= 1, ++j) anc[u][j] = stack[d - i];

	for (int i = first[u]; i != -1; i = edge[i].next)
		if (depth[edge[i].v] == -1) DFS(edge[i].v, d + 1);
}

int ancestor(int u, int k) {
	while (k > 0) u = anc[u][Log[k]], k -= 1 << Log[k];
	return u;
}

int get_LCA(int u, int v) {
	int low = 0, high = min(depth[u], depth[v]);
	u = ancestor(u, depth[u] - high); v = ancestor(v, depth[v] - high);
	if (u == v) return u;
	while (low + 1 < high) {
		int lvl = Log[high - low - 1], lu = anc[u][lvl], lv = anc[v][lvl], mid = high - (1 << lvl);
		if (lu == lv) low = mid;
		else high = mid, u = lu, v = lv;
	}
	return anc[u][Log[high - low]];
}

int LCA_Online() {
	Log[1] = 0;
	for (int j = 1, i = 2; i < N; ++i)
		if ((1 << j) == i) Log[i] = Log[i - 1] + 1, ++j;
		else Log[i] = Log[i - 1];

	memset(depth, -1, sizeof(depth));
	DFS(0, 0);
}
