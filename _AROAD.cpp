/*
	Find a minimum spanning tree whose vertex 1 has a degree limit D
*/
#include <cstdio>
#include <cstring>
#include <algorithm>

using namespace std;

const int MAXN = 1000 + 1; //number of vertices + 1
const int MAXM = 100000; //number of edges
const int INF = 2000000000;

struct Tedge {
	int v, w, next;
};

Tedge edge[MAXM * 2], mst_edge[MAXM * 2];
int first[MAXN], mst_first[MAXN], dist[MAXN], heap[MAXN], pos[MAXN], maxw[MAXN], path[MAXN], prev[MAXN];
bool used[MAXN];
int N, M, D, cnt, num, ans;

inline void add_edge(Tedge& e, int& first, int i, int v, int w) {
	e.v = v; e.w = w; e.next = first; first = i;
}

void init() {
	memset(first, -1, sizeof(first));
	scanf("%d%d%d", &N, &M, &D);
	for (int i = 0; i < M; ++i) {
		int u, v, w;
		scanf("%d%d%d", &u, &v, &w);
		--u; --v;
		add_edge(edge[i * 2], first[u], i * 2, v, w);
		add_edge(edge[i * 2 + 1], first[v], i * 2 + 1, u, w);
	}
}

inline void moveup(int i) {
	int key = heap[i];
	while (i > 1 && dist[heap[i >> 1]] > dist[key])	heap[i] = heap[i >> 1], pos[heap[i]] = i, i >>= 1;
	heap[i] = key; pos[key] = i;
}

inline void movedown(int i) {
	int key = heap[i];
	while ((i << 1) <= num) {
		int j = i << 1;
		if (j < num && dist[heap[j + 1]] < dist[heap[j]]) ++j;
		if (dist[key] <= dist[heap[j]]) break;
		heap[i] = heap[j]; pos[heap[i]] = i; i = j;
	}
	heap[i] = key; pos[key] = i;
}

void Prim(int u) {
	int minw = INF, s;

	num = 0;
	while (1) {
		used[u] = 1;
		for (int i = first[u]; i != -1; i = edge[i].next) {
			int v = edge[i].v, w = edge[i].w;
			if (!used[v] && (dist[v] == -1 || w < dist[v])) {
				dist[v] = w;
				prev[v] = u;
				if (pos[v] == -1) pos[v] = ++num, heap[num] = v;
				moveup(pos[v]);
			}
			else if (used[v] && v == 0 && w < minw) minw = w, s = i;
		}

		if (!num) break;
		u = heap[1]; heap[1] = heap[num--]; movedown(1);
		ans += dist[u];
		add_edge(mst_edge[cnt], mst_first[u], cnt, prev[u], dist[u]); ++cnt;
		add_edge(mst_edge[cnt], mst_first[prev[u]], cnt, u, dist[u]); ++cnt;
	}

	if (minw == INF) return;
	edge[s].w = -1; edge[s ^ 1].w = -1;
	s = edge[s ^ 1].v; ans += minw; --D;
	add_edge(mst_edge[cnt], mst_first[0], cnt, s, minw); ++cnt;
	add_edge(mst_edge[cnt], mst_first[s], cnt, 0, minw); ++cnt;
}

void DFS(int u) {
	used[u] = 1;
	for (int i = mst_first[u]; i != -1; i = mst_edge[i].next) {
		int v = mst_edge[i].v, w = mst_edge[i].w;
		if (w > -1 && !used[v]) {
			if (w > maxw[v]) maxw[v] = w, path[v] = i;
			if (maxw[u] > maxw[v]) maxw[v] = maxw[u], path[v] = path[u];
			DFS(v);
		}
	}
}

void work() {
	ans = cnt = 0;
	memset(mst_first, -1, sizeof(mst_first));
	memset(dist, -1, sizeof(dist));
	memset(pos, -1, sizeof(pos));
	memset(used, 0, sizeof(used));
	used[0] = 1;
	for (int i = first[0]; i != -1; i = edge[i].next)
		if (!used[edge[i].v]) Prim(edge[i].v);

	if (D < 0) {
		printf("NONE\n");
		return;
	}
	for (int i = 1; i < N; ++i)
		if (!used[i]) {
			printf("NONE\n");
			return;
		}

	memset(maxw, -1, sizeof(maxw));
	memset(used, 0, sizeof(used));
	used[0] = 1;
	for (int i = mst_first[0]; i != -1; i = mst_edge[i].next) DFS(mst_edge[i].v);
	for (int i = 0; i < D; ++i) {
		int minw = INF, s, x, y;
		for (int j = first[0]; j != -1; j = edge[j].next) {
			int v = edge[j].v, w = edge[j].w;
			if (w > -1 && maxw[v] > -1 && w - maxw[v] < minw) {
				minw = w - maxw[v]; s = v;
				x = path[v]; y = j;
			}
		}
		if (minw >= 0) break;

		ans += minw;
		mst_edge[x].w = mst_edge[x ^ 1].w = -1;
		add_edge(mst_edge[cnt], mst_first[0], cnt, s, edge[y].w); ++cnt;
		add_edge(mst_edge[cnt], mst_first[s], cnt, 0, edge[y].w); ++cnt;
		edge[y].w = edge[y ^ 1].w = -1;

		memset(used, 0, sizeof(used));
		used[0] = 1;
		for (int u = 0; u < N; ++u)
			if (path[u] == x) maxw[u] = -1;
		DFS(s);
	}

	printf("%d\n", ans);
}

int main() {
	int c;
	for (scanf("%d", &c); c > 0; --c) {
		init();
		work();
	}
	return 0;
}
